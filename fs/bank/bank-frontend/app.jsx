import { Routes, Route, Navigate } from 'react-router-dom';
import { createTheme, ThemeProvider, CssBaseline } from '@mui/material';

import BankLayout from './layouts/BankLayout.jsx';

import Login from './pages/Login.jsx';
import Signup from './pages/Signup.jsx';
import Dashboard from './pages/Dashboard.jsx';
import Transfer from './pages/Transfer.jsx';
import Verify from './pages/Verify.jsx';
import ResetPassword from './pages/ResetPassword.jsx';

import ProtectedRoute from './components/protectedRoute.jsx';

const theme = createTheme({
  palette: {
    primary: { main: '#0c4a6e' },
    secondary: { main: '#c2410c' },
    background: { default: '#f7f5f1' }
  },
  typography: {
    fontFamily: '"Space Grotesk", "Helvetica Neue", Arial, sans-serif'
  },
  shape: { borderRadius: 14 }
});

export default function App() {
  return (
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <Routes>
        <Route element={<BankLayout />}>
          <Route path="/" element={<Navigate to="/login" replace />} />

          <Route path="/login" element={<Login />} />
          <Route path="/register" element={<Signup />} />
          <Route path="/verify" element={<Verify />} />
          <Route path="/reset-password" element={<ResetPassword />} />

          <Route element={<ProtectedRoute />}>
            <Route path="/dashboard" element={<Dashboard />} />
            <Route path="/transfer" element={<Transfer />} />
          </Route>
        </Route>
      </Routes>
    </ThemeProvider>
  );
}
