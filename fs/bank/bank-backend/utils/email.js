import nodemailer from 'nodemailer';
import path from 'path';


/* ======================
   Email Layout (GLOBAL)
====================== */
const emailLayout = (content) => `
  <div style="background:#f5f7fa; padding:40px 0; font-family:Arial, sans-serif">
    <div style="
      max-width:600px;
      margin:auto;
      background:#ffffff;
      border-radius:10px;
      overflow:hidden;
      box-shadow:0 4px 12px rgba(0,0,0,0.08)
    ">

      <div style="
        background:linear-gradient(90deg,#1a73e8,#0b4fa2);
        padding:20px;
        text-align:center;
      ">
        <img
          src="cid:bank-logo"
          alt="Bank One One"
          style="max-width:140px"
        />
      </div>

      <div style="padding:30px">
        ${content}
      </div>

      <div style="
        background:#f0f2f5;
        padding:16px;
        text-align:center;
        font-size:12px;
        color:#666
      ">
        © ${new Date().getFullYear()} Bank One One · Secure Banking
      </div>

    </div>
  </div>
`;


/* ======================
   Verification Email
====================== */

export const sendVerificationEmail = async (email, token) => {
  console.log('📨 Verification token:', token);
  
  const verificationLink = `${process.env.APP_BASE_URL}/verify?token=${token}`;

  const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
      user: process.env.MAIL_USER,
      pass: process.env.MAIL_PASS
    }
  });

  const content = `
    <h2 style="color:#1a73e8; text-align:center">
      Verify your account
    </h2>

    <p>Hello <strong>${email}</strong>,</p>

    <p>
      Welcome to <strong>Bank One One</strong>.
      Please verify your email to activate your account.
    </p>

    <div style="text-align:center; margin:30px 0">
      <a
        href="${verificationLink}"
        style="
          background:#1a73e8;
          color:#ffffff;
          padding:14px 26px;
          border-radius:6px;
          text-decoration:none;
          font-size:16px;
        "
      >
        Verify Account
      </a>
    </div>
  `;

  await transporter.sendMail({
    from: '"Bank One One" <no-reply@bankoneone.com>',
    to: email,
    subject: 'Verify your Bank One One account',
    html: emailLayout(content),
    attachments: [
      {
        filename: 'bank-one-one-logo.png',
        path: path.join(process.cwd(), 'logo', 'bank-one-one-logo.png'),
        cid: 'bank-logo' 
      }
    ]
  });
};

/* ======================
   Password Reset Email
====================== */

export const sendPasswordResetEmail = async (email, token) => {
  console.log('🔐 Reset token:', token);

  const resetLink = `${process.env.APP_BASE_URL}/reset-password?token=${token}`;

  const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
      user: process.env.MAIL_USER,
      pass: process.env.MAIL_PASS
    }
  });

  const content = `
    <h2 style="color:#1a73e8; text-align:center">
      Reset your password
    </h2>

    <p>Hello <strong>${email}</strong>,</p>

    <p>
      We received a request to reset your password.
      Click the button below to set a new password.
    </p>

    <div style="text-align:center; margin:30px 0">
      <a
        href="${resetLink}"
        style="
          background:#1a73e8;
          color:#ffffff;
          padding:14px 26px;
          border-radius:6px;
          text-decoration:none;
          font-size:16px;
        "
      >
        Reset Password
      </a>
    </div>
  `;

  await transporter.sendMail({
    from: '"Bank One One" <no-reply@bankoneone.com>',
    to: email,
    subject: 'Reset your Bank One One password',
    html: emailLayout(content),
    attachments: [
      {
        filename: 'bank-one-one-logo.png',
        path: path.join(process.cwd(), 'logo', 'bank-one-one-logo.png'),
        cid: 'bank-logo'
      }
    ]
  });
};
