import bcrypt from 'bcrypt';
import jwt from 'jsonwebtoken';
import { randomUUID } from 'crypto';

import usersModel from '../models/usersModel.js';
import accountsModel from '../models/accountsModel.js';
import { isValidEmail, isValidPhone } from '../utils/validators.js';
import { sendVerificationEmail } from '../utils/email.js';
import { sendPasswordResetEmail } from '../utils/email.js';
import { JWT_SECRET } from '../middleware/auth.js';

/* ================= REGISTER ================= */
const signup = async (req, res) => {
  try {
    const { email, password, phoneNumber, firstName, lastName } = req.body;

    if (!email || !password || !phoneNumber || !firstName || !lastName) {
      return res.status(400).json({ message: 'Missing fields' });
    }

    if (!isValidEmail(email)) {
      return res.status(400).json({ message: 'Invalid email format' });
    }

    if (!isValidPhone(phoneNumber)) {
      return res.status(400).json({ message: 'Invalid phone number format' });
    }

    const existingUser = await usersModel.findUserByEmail(email);
    if (existingUser) {
      return res.status(409).json({ message: 'Email already registered' });
    }

    const passwordHash = await bcrypt.hash(password, 10);
    const verificationToken = randomUUID();

    const user = await usersModel.createUser({
      firstName,
      lastName,
      email,
      phoneNumber,
      password: passwordHash,
      isVerified: false,
      verificationToken,
      verificationExpires: Date.now() + 24 * 60 * 60 * 1000
    });

    await accountsModel.createAccount(user._id);

    await sendVerificationEmail(email, verificationToken);

    return res.status(201).json({
      message: 'Registration successful. Please verify your email.'
    });

  } catch (err) {
    console.error(err);
    return res.status(500).json({ message: 'Server error' });
  }
};

/* ================= VERIFY ================= */
const verify = async (req, res) => {
  try {
    const { token: rawToken } = req.query;
    const token = Array.isArray(rawToken) ? rawToken[0] : rawToken;

    if (!token || typeof token !== 'string' || !token.trim()) {
      return res.status(400).json({ message: 'Token is required' });
    }

    const normalizedToken = token.trim();

    const user = await usersModel.findUserByVerificationToken(normalizedToken);
    if (!user) {
      return res.status(404).json({ message: 'Invalid token' });
    }

    if (user.isVerified) {
      return res.status(400).json({ message: 'User already verified' });
    }

    if (Date.now() > user.verificationExpires) {
      return res.status(400).json({ message: 'Token expired' });
    }

    user.isVerified = true;
    user.verificationToken = null;
    user.verificationExpires = null;
    await user.save();

    const account = await accountsModel.findAccountByUserId(user._id);
    if (account) 
    {
      await accountsModel.updateAccountStatus(account._id, 'ACTIVE');
    }

    const accessToken = jwt.sign(
      {
        userId: user._id,
        email: user.email
      },
      JWT_SECRET,
      { expiresIn: '1h' }
    );

    return res.status(200).json({
      message: 'Account verified successfully',
      accessToken
    });

  } catch (err) {
    console.error(err);
    return res.status(500).json({ message: 'Server error' });
  }
};

/* ================= LOGIN ================= */
const login = async (req, res) => {
  try {
    const { email, password } = req.body;

    if (!email || !password) {
      return res.status(400).json({ message: 'Email and password required' });
    }

    const user = await usersModel.findUserByEmailWithPassword(email);

    if (!user) {
      return res.status(401).json({ message: 'User not registered' });
    }

    if (!user.isVerified) {
      return res.status(403).json({ message: 'Account not verified' });
    }

    const match = await bcrypt.compare(password, user.password);
    if (!match) {
      return res.status(401).json({ message: 'Invalid credentials' });
    }

    const accessToken = jwt.sign(
      {
        userId: user._id,
        email: user.email
      },
      JWT_SECRET,
      { expiresIn: '1h' }
    );

    return res.status(200).json({
      accessToken
    });

  } catch (err) {
    console.error(err);
    return res.status(500).json({ message: 'Server error' });
  }
};

/* ================= LOGOUT ================= */

const logout = (req, res) => {
  return res.status(200).json({
    message: 'Logged out successfully'
  });
};

/* ================= FORGOT PASSWORD ================= */
const forgotPassword = async (req, res) => {
  try {
    const { email } = req.body;

    if (!email) {
      return res.status(400).json({ message: 'Email is required' });
    }

    const user = await usersModel.findUserByEmail(email);
    if (!user) {
      return res.status(404).json({ message: 'User not registered' });
    }

    const resetToken = randomUUID();
    user.resetPasswordToken = resetToken;
    user.resetPasswordExpires = Date.now() + 60 * 60 * 1000;
    await user.save();

    await sendPasswordResetEmail(email, resetToken);

    return res.status(200).json({
      message: 'Password reset email sent'
    });
  } catch (err) {
    console.error(err);
    return res.status(500).json({ message: 'Server error' });
  }
};

/* ================= RESET PASSWORD ================= */
const resetPassword = async (req, res) => {
  try {
    const { token, password, confirmPassword } = req.body;

    if (!token || !password || !confirmPassword) {
      return res.status(400).json({ message: 'Missing fields' });
    }

    if (password !== confirmPassword) {
      return res.status(400).json({ message: 'Passwords do not match' });
    }

    const user = await usersModel.findUserByResetToken(token);
    if (!user) {
      return res.status(404).json({ message: 'Invalid or expired token' });
    }

    if (Date.now() > user.resetPasswordExpires) {
      return res.status(400).json({ message: 'Token expired' });
    }

    user.password = await bcrypt.hash(password, 10);
    user.resetPasswordToken = null;
    user.resetPasswordExpires = null;
    await user.save();

    return res.status(200).json({
      message: 'Password updated successfully'
    });
  } catch (err) {
    console.error(err);
    return res.status(500).json({ message: 'Server error' });
  }
};

export default {
  signup,
  verify,
  login,
  logout,
  forgotPassword,
  resetPassword
};
