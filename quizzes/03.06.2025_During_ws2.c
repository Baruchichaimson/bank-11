#include <stdio.h>
#include <string.h>
#include <stddef.h>

void TF(int num)
{
	int i;
	for(i = 1; i <= num; i++)
	{	
		if (0 == i % 3 && 0 == i % 5)
		{
			printf("TF\n");
		}
		else if (0 == i % 3)
		{
			printf("T\n");
		}
		else if (0 == i % 5)
		{
			printf("F\n");
		}
		else if(i % 3 != 0 && i % 5 != 0)
		{
			printf("%d\n", i);
		}
	}
}

void reverses(char *str) {
    int size = strlen(str);
    char *start = str;
    char *end = str + size - 1;
    char temp;

    while (start < end) {
     
        if (*start >= 'A' && *start <= 'Z') {
            *start = *start + ('a' -'A');
        }
        if (*end >= 'A' && *end <= 'Z') {
            *end = *end + ('a' -'A');
        }

        temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}
void TF_Test()
{
	TF(15);
	
}

void reverses_Test()
{
	char str[] = "jhfdshGJHWQwh87984#$#@$gjhewg";      
	
	printf("Input:  %s\n", str);
	
	reverses(str);
	
	printf("Output: %s\n", str);
}

int main()
{
	printf("Test TF_Test:\n");
	TF_Test();
	
	printf("Test ReverseStrAndLowCaseTest:\n");
	reverses_Test();
	
	return 0;
}



// server.js
const express = require('express');
const cors = require('cors');
const swaggerUi = require('swagger-ui-express');
const swaggerJsdoc = require('swagger-jsdoc');

const app = express();
const PORT = process.env.PORT || 5000;

const jwt = require('jsonwebtoken');
const JWT_SECRET = 'my_super_secret_key';

const bcrypt = require('bcrypt');
const { randomUUID } = require('crypto');

const nodemailer = require('nodemailer');

app.use(cors());
app.use(express.json());

function isValidEmail(email) {
  return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email);
}

function isValidPhone(phone) {
  return /^05\d{8}$/.test(phone);
}

function authenticateToken(req, res, next) {
  const authHeader = req.headers['authorization'];
  const token = authHeader && authHeader.split(' ')[1];

  if (!token) 
  {
    return res.status(401).json({ message: 'Token missing' });
  }

  jwt.verify(token, JWT_SECRET, (err, user) => {
    if (err) 
    {
      return res.status(403).json({ message: 'Invalid token' });
    }
    req.user = user; 
    next();
  });
}

function authenticateVerifiedToken(req, res, next) {
  const authHeader = req.headers['authorization'];
  const token = authHeader && authHeader.split(' ')[1];

  if (!token) 
    return res.status(403).json({ message: 'You must click the verification link first' }); 

  jwt.verify(token, JWT_SECRET, (err, userPayload) => {
    if (err) return res.status(403).json({ message: 'Invalid token' });

    const user = users[userPayload.email];
    if (!user) return res.status(404).json({ message: 'User not found' });

    if (!user.isVerified) 
      return res.status(403).json({ message: 'You must click the verification link first' }); 

    req.user = userPayload; 
    next();
  });
}

async function sendVerificationEmail(email, token) {
  const transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
      user: 'chaimsonb@gmail.com',
      pass: 'prlo xcmu zimr uewg'
    }
  });

  const os = require('os');
  const interfaces = os.networkInterfaces();
  let localIp = '127.0.0.1';
  for (let iface of Object.values(interfaces)) {
    for (let i of iface) {
      if (i.family === 'IPv4' && !i.internal) {
        localIp = i.address;
        break;
      }
    }
  }

  const verificationLink = `http://${localIp}:${PORT}/api/auth/verify?token=${token}`;

  await transporter.sendMail({
    from: '"Bank App Team" <chaimsonb@gmail.com>',
    to: email,
    replyTo: 'chaimsonb@gmail.com',
    subject: 'Verify your account',
    html: `
      <p>Hello ${email},</p>
      <p>Click the link below to verify your account:</p>
      <p><a href="${verificationLink}" target="_blank">${verificationLink}</a></p>
      <p>If you did not sign up, ignore this email.</p>
    `
  });

  console.log(`Verification email sent to ${email}: ${verificationLink}`);
}


/* =========================
   Swagger Configuration
========================= */

const swaggerOptions = {
  definition: {
    openapi: '3.0.0',
    info: {
      title: 'Web Banking API',
      version: '1.0.0',
      description: 'API for Web Banking Application',
    },
    servers: [
      {
        url: `http://localhost:${PORT}`,
        description: 'Bank One One',
      },
    ],
    components: {
      securitySchemes: {
        bearerAuth: {
          type: 'http',
          scheme: 'bearer',
          bearerFormat: 'JWT',
        },
      },
      schemas: {
        /* ================= USER ================= */
        User: {
          type: 'object',
          properties: {
            userId: { type: 'string' },
            email: { type: 'string', format: 'email' },
            phoneNumber: { type: 'string' },
            isVerified: { type: 'boolean' },
            createdAt: { type: 'string', format: 'date-time' },
          },
        },

        /* ================= ACCOUNT ================= */
        Account: {
          type: 'object',
          properties: {
            accountId: { type: 'string' },
            userId: { type: 'string' },
            balance: { type: 'number' },
            createdAt: { type: 'string', format: 'date-time' },
            updatedAt: { type: 'string', format: 'date-time' },
          },
        },

        /* ================= TRANSACTION ================= */
        Transaction: {
          type: 'object',
          properties: {
            transactionId: { type: 'string' },
            senderAccountId: { type: 'string' },
            receiverAccountId: { type: 'string' },
            amount: { type: 'number' },
            transactionType: {
              type: 'string',
              enum: ['TRANSFER'],
            },
            status: {
              type: 'string',
              enum: ['PENDING', 'COMPLETED', 'FAILED'],
            },
            createdAt: { type: 'string', format: 'date-time' },
          },
        },

        TransactionRequest: {
          type: 'object',
          required: ['receiverEmail', 'amount'],
          properties: {
            receiverEmail: {
              type: 'string',
              format: 'email',
            },
            amount: {
              type: 'number',
              minimum: 1,
            },
          },
        },

        /* ================= VERIFICATION ================= */
        VerificationRequest: {
          type: 'object',
          required: ['email', 'code'],
          properties: {
            email: { type: 'string', format: 'email' },
            code: { type: 'string', example: '123456' },
          },
        },

        /* ================= AUTH ================= */
        RegisterRequest: {
          type: 'object',
          required: ['email', 'password', 'phoneNumber'],
          properties: {
            email: { type: 'string', format: 'email' },
            password: { type: 'string', format: 'password' },
            phoneNumber: { type: 'string' },
          },
        },

        LoginRequest: {
          type: 'object',
          required: ['email', 'password'],
          properties: {
            email: { type: 'string', format: 'email' },
            password: { type: 'string', format: 'password' },
          },
        },

        Error: {
          type: 'object',
          properties: {
            message: { type: 'string' },
          },
        },
      },
    },
    security: [{ bearerAuth: [] }],
  },
  apis: ['./server.js'],
};

/* =========================
   In-memory accounts
========================= */
const users = {};
const accounts = {
  ACC001: { accountId: 'ACC001', userId: 'USER001', balance: 1000, createdAt: new Date(), updatedAt: new Date() },
  ACC002: { accountId: 'ACC002', userId: 'USER002', balance: 500, createdAt: new Date(), updatedAt: new Date() }
};

const transactionsStore = [];

const swaggerDocs = swaggerJsdoc(swaggerOptions);
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocs));

/* =========================
   API Routes (Mock only)
========================= */

/**
 * @swagger
 * /api/health:
 *   get:
 *     summary: Health check
 *     tags: [System]
 *     responses:
 *       200:
 *         description: Server is running
 */
app.get('/api/health', (req, res) => {
  res.json({ status: 'OK', time: new Date().toISOString() });
});

/* ================= AUTH ================= */

/**
 * @swagger
 * /api/auth/register:
 *   post:
 *     summary: Register new user
 *     tags: [Auth]
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             $ref: '#/components/schemas/RegisterRequest'
 *     responses:
 *       201:
 *         description: User registered successfully. Login to receive verification email.
 *       400:
 *         description: Invalid input
 *       409:
 *         description: Email already registered
 */

app.post('/api/auth/register', async (req, res) => {
  try {
    const { email, password, phoneNumber } = req.body;

    if (!email || !password || !phoneNumber)
      return res.status(400).json({ message: 'Missing fields' });

    if (!isValidEmail(email))
      return res.status(400).json({ message: 'Invalid email format' });

    if (!isValidPhone(phoneNumber))
      return res.status(400).json({ message: 'Invalid phone number format' });

    if (users[email])
      return res.status(409).json({ message: 'Email already registered' });

    const passwordHash = await bcrypt.hash(password, 10);
    const userId = randomUUID();

    users[email] = {
      userId,
      email,
      passwordHash,
      phoneNumber,
      isVerified: false, 
      createdAt: new Date()
    };

    accounts[userId] = {
      accountId: randomUUID(),
      userId,
      balance: Math.floor(Math.random() * 9000) + 1000,
      createdAt: new Date(),
      updatedAt: new Date()
    };

    res.status(201).json({
      message: 'User registered successfully. Please login to receive verification email.'
    });
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: 'Server error' });
  }
});



/**
 * @swagger
 * /api/auth/verify:
 *   get:
 *     summary: Verify user account via token
 *     tags: [Auth]
 *     parameters:
 *       - in: query
 *         name: token
 *         required: true
 *         schema:
 *           type: string
 *         description: Verification token sent by email
 *     responses:
 *       200:
 *         description: Account verified successfully with link to dashboard
 *       400:
 *         description: Token missing, expired, or user already verified
 *       404:
 *         description: Invalid token
 */
app.get('/api/auth/verify', (req, res) => {
  const { token } = req.query;

  if (!token) return res.status(400).send('Token is required');

  const user = Object.values(users).find(u => u.verificationToken === token);

  if (!user) return res.status(404).send('Invalid token');
  if (user.isVerified) return res.status(400).send('User already verified');
  if (Date.now() > user.verificationExpires) return res.status(400).send('Token expired');

  user.isVerified = true;
  delete user.verificationToken;
  delete user.verificationExpires;

  res.send('Account verified successfully! You can now access your account data.');
});

/**
 * @swagger
 * /api/auth/login:
 *   post:
 *     summary: Login user
 *     tags: [Auth]
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             $ref: '#/components/schemas/LoginRequest'
 *     responses:
 *       200:
 *         description: JWT token returned
 *       400:
 *         description: Email and password required
 *       401:
 *         description: Invalid credentials or user not registered
 *       403:
 *         description: Account not verified. Verification link sent to email
 */
app.post('/api/auth/login', async (req, res) => {
  try {
    const { email, password } = req.body;

    if (!email || !password)
      return res.status(400).json({ message: 'Email and password required' });

    const user = users[email];
    if (!user) return res.status(401).json({ message: 'User not registered' });

    const match = await bcrypt.compare(password, user.passwordHash);
    if (!match) return res.status(401).json({ message: 'Invalid credentials' });

    if (!user.isVerified) {
      const verificationToken = randomUUID();
      user.verificationToken = verificationToken;
      user.verificationExpires = Date.now() + 24 * 60 * 60 * 1000; 

      await sendVerificationEmail(email, verificationToken);

      return res.status(403).json({
        message: 'Account not verified. Verification link sent to your email.'
      });
    }

    const payload = { userId: user.userId, email: user.email };
    const token = jwt.sign(payload, JWT_SECRET, { expiresIn: '1h' });

    res.json({ token });
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: 'Server error' });
  }
});



/**
 * @swagger
 * /api/auth/logout:
 *   post:
 *     summary: Logout user
 *     tags: [Auth]
 *     security:
 *       - bearerAuth: []
 *     responses:
 *       200:
 *         description: Logged out successfully
 */
app.post('/api/auth/logout', (req, res) => {
  res.json({ message: 'Logged out successfully' });
});

/* ================= ACCOUNT ================= */

/**
 * @swagger
 * /api/account:
 *   get:
 *     summary: Get user account details
 *     tags: [Account]
 *     security:
 *       - bearerAuth: []
 *     responses:
 *       200:
 *         description: Account data with transactions
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 account:
 *                   $ref: '#/components/schemas/Account'
 *                 transactions:
 *                   type: array
 *                   items:
 *                     $ref: '#/components/schemas/Transaction'
 *                 message:
 *                   type: string
 */
app.get('/api/account', (req, res) => {
  let user;

  // אם יש JWT, נבדוק כמו קודם
  if (req.headers['authorization']) {
    const authHeader = req.headers['authorization'];
    const token = authHeader.split(' ')[1];
    try {
      const payload = jwt.verify(token, JWT_SECRET);
      user = users[payload.email];
    } catch {
      return res.status(403).json({ message: 'Invalid token' });
    }
  } 

  // אם אין JWT, נבדוק אם יש משתמש עם isVerified = true – לחיבור ישיר מהאימות
  if (!user) {
    user = Object.values(users).find(u => u.isVerified === true);
    if (!user) return res.status(403).json({ message: 'You must verify your account first' });
  }

  const userId = user.userId;

  const account = Object.values(accounts).find(acc => acc.userId === userId);
  if (!account) return res.status(404).json({ message: 'Account not found' });

  const transactions = [
    {
      transactionId: 'TX001',
      senderAccountId: account.accountId,
      receiverAccountId: 'ACC002',
      amount: 100,
      transactionType: 'TRANSFER',
      status: 'COMPLETED',
      createdAt: new Date().toISOString()
    },
    {
      transactionId: 'TX002',
      senderAccountId: 'ACC002',
      receiverAccountId: account.accountId,
      amount: 250,
      transactionType: 'TRANSFER',
      status: 'COMPLETED',
      createdAt: new Date().toISOString()
    }
  ];

  res.json({
    message: `Welcome ${user.email}!`,
    account,
    transactions
  });
});





/* ================= TRANSACTIONS ================= */

/**
 * @swagger
 * /api/transactions:
 *   post:
 *     summary: Transfer money to another account
 *     tags: [Transactions]
 *     security:
 *       - bearerAuth: []
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             required:
 *               - receiverAccountId
 *               - amount
 *             properties:
 *               receiverAccountId:
 *                 type: string
 *                 example: "ACC002"
 *               amount:
 *                 type: number
 *                 example: 100
 *     responses:
 *       201:
 *         description: Transaction completed and balances updated
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 message:
 *                   type: string
 *                   example: "Transaction completed"
 *                 senderBalance:
 *                   type: number
 *                   example: 900
 *                 receiverBalance:
 *                   type: number
 *                   example: 600
 *       400:
 *         description: Insufficient funds or account not found
 *         content:
 *           application/json:
 *             schema:
 *               $ref: '#/components/schemas/Error'
 */

app.post('/api/transactions', authenticateVerifiedToken, (req, res) => {
  const { receiverEmail, amount } = req.body;
  const senderEmail = req.user.email;

  if (!receiverEmail || !amount) {
    return res.status(400).json({ message: 'receiverEmail and amount required' });
  }

  if (!users[senderEmail] || !users[receiverEmail]) {
    return res.status(400).json({ message: 'User not found' });
  }

  const senderAccount = accounts[users[senderEmail].userId];
  const receiverAccount = accounts[users[receiverEmail].userId];

  if (senderAccount.balance < amount) {
    return res.status(400).json({ message: 'Insufficient funds' });
  }

  senderAccount.balance -= amount;
  senderAccount.updatedAt = new Date();

  receiverAccount.balance += amount;
  receiverAccount.updatedAt = new Date();

  const transaction = {
    transactionId: randomUUID(),
    senderAccountId: senderAccount.accountId,
    receiverAccountId: receiverAccount.accountId,
    senderEmail,
    receiverEmail,
    amount,
    transactionType: 'TRANSFER',
    status: 'COMPLETED',
    createdAt: new Date().toISOString()
  };

  transactionsStore.push(transaction);

  res.status(201).json({
    message: 'Transaction completed',
    senderBalance: senderAccount.balance,
    receiverBalance: receiverAccount.balance,
    transaction
  });
});



/**
 * @swagger
 * /api/transactions:
 *   get:
 *     summary: Get transaction history
 *     tags: [Transactions]
 *     security:
 *       - bearerAuth: []
 *     responses:
 *       200:
 *         description: List of transactions
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 $ref: '#/components/schemas/Transaction'
 */
app.get('/api/transactions', authenticateToken, (req, res) => {
  const userEmail = req.user.email;

  const userTransactions = transactionsStore.filter(
    t => t.senderEmail === userEmail || t.receiverEmail === userEmail
  ).map(t => ({
    ...t,
    sign: t.senderEmail === userEmail ? '-' : '+'
  }));

  res.json(userTransactions);
});


/**
 * @swagger
 * /api/transactions/{transactionId}:
 *   get:
 *     summary: Get transaction details by ID
 *     tags: [Transactions]
 *     security:
 *       - bearerAuth: []
 *     parameters:
 *       - in: path
 *         name: transactionId
 *         required: true
 *         schema:
 *           type: string
 *     responses:
 *       200:
 *         description: Transaction details
 *         content:
 *           application/json:
 *             schema:
 *               $ref: '#/components/schemas/Transaction'
 *       404:
 *         description: Transaction not found
 */
app.get('/api/transactions/:transactionId', (req, res) => {
  const { transactionId } = req.params;
  // Mock response example
  res.json({
    transactionId,
    senderAccountId: 'ACC123',
    receiverAccountId: 'ACC456',
    amount: 100,
    transactionType: 'TRANSFER',
    status: 'COMPLETED',
    createdAt: new Date().toISOString(),
  });
});

/* ========================= */

app.listen(PORT, '0.0.0.0', () => {
  const os = require('os');
  const interfaces = os.networkInterfaces();
  let ip = 'localhost';

  for (let iface of Object.values(interfaces)) {
    for (let i of iface) {
      if (i.family === 'IPv4' && !i.internal) {
        ip = i.address;
        break;
      }
    }
  }

  console.log(`Server running on http://${ip}:${PORT}`);
  console.log(`Swagger docs: http://${ip}:${PORT}/api-docs`);
});

module.exports = app;

תחלק לי את זה לקבצים כפי שהדרישה אז
ובנוסף תסביר לי לגבי jtw מאיפה אני משיג את הטוקן?