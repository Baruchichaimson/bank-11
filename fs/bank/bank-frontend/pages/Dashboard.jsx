import {
  Box,
  Button,
  Chip,
  Container,
  Grid,
  Paper,
  Stack,
  Typography
} from '@mui/material';
import ArrowOutwardIcon from '@mui/icons-material/ArrowOutward';
import AddCardIcon from '@mui/icons-material/AddCard';
import SavingsIcon from '@mui/icons-material/Savings';
import SwapHorizIcon from '@mui/icons-material/SwapHoriz';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '../context/AuthContext.jsx';

export default function Dashboard() {
  const { account, transactions, loading } = useAuth();
  const navigate = useNavigate();

  if (loading) {
    return (
      <Box sx={{ py: 8, textAlign: 'center' }}>
        <Typography>Loading dashboard...</Typography>
      </Box>
    );
  }

  if (!account) {
    return null;
  }

  return (
    <Box sx={{ py: { xs: 6, md: 8 } }}>
      <Container maxWidth="lg">
        {/* ===== Header ===== */}
        <Stack
          direction={{ xs: 'column', md: 'row' }}
          justifyContent="space-between"
          alignItems={{ xs: 'flex-start', md: 'center' }}
          spacing={2}
          sx={{ mb: 4 }}
        >
          <Box>
            <Typography variant="overline" color="secondary.main">
              Dashboard
            </Typography>
            <Typography variant="h4" sx={{ fontWeight: 600 }}>
              Welcome back 👋
            </Typography>
            <Typography color="text.secondary">
              Here is your financial overview for today.
            </Typography>
          </Box>

          <Button
            variant="contained"
            endIcon={<ArrowOutwardIcon />}
            onClick={() => navigate('/transfer')}
          >
            New transfer
          </Button>
        </Stack>

        {/* ===== Cards ===== */}
        <Grid container spacing={3}>
          <Grid item xs={12} md={4}>
            <Paper sx={{ p: 3, height: '100%' }}>
              <Typography variant="subtitle2" color="text.secondary">
                Main balance
              </Typography>
              <Typography variant="h3" sx={{ mt: 1, fontWeight: 600 }}>
                ₪{account.balance.toLocaleString()}
              </Typography>
              <Chip
                label={account.status}
                color="secondary"
                size="small"
                sx={{ mt: 2 }}
              />
            </Paper>
          </Grid>

          <Grid item xs={12} md={4}>
            <Paper sx={{ p: 3, height: '100%' }}>
              <Stack direction="row" spacing={1} alignItems="center">
                <SavingsIcon color="primary" />
                <Typography variant="subtitle2" color="text.secondary">
                  Savings
                </Typography>
              </Stack>
              <Typography variant="h5" sx={{ mt: 2, fontWeight: 600 }}>
                ₪0
              </Typography>
              <Typography color="text.secondary" sx={{ mt: 1 }}>
                Feature coming soon
              </Typography>
            </Paper>
          </Grid>

          <Grid item xs={12} md={4}>
            <Paper sx={{ p: 3, height: '100%' }}>
              <Stack direction="row" spacing={1} alignItems="center">
                <AddCardIcon color="primary" />
                <Typography variant="subtitle2" color="text.secondary">
                  Cards
                </Typography>
              </Stack>
              <Typography variant="h5" sx={{ mt: 2, fontWeight: 600 }}>
                —
              </Typography>
              <Typography color="text.secondary" sx={{ mt: 1 }}>
                Cards management coming soon
              </Typography>
            </Paper>
          </Grid>
        </Grid>

        {/* ===== Transactions ===== */}
        <Grid container spacing={3} sx={{ mt: 1 }}>
          <Grid item xs={12} md={7}>
            <Paper sx={{ p: 3 }}>
              <Typography variant="h6" sx={{ mb: 2, fontWeight: 600 }}>
                Latest activity
              </Typography>

              {transactions.length === 0 ? (
                <Typography color="text.secondary">
                  No transactions yet
                </Typography>
              ) : (
                <Stack spacing={2}>
                  {transactions.slice(0, 5).map((tx) => (
                    <Stack
                      key={tx._id || tx.id}
                      direction="row"
                      justifyContent="space-between"
                      alignItems="center"
                    >
                      <Box>
                        <Typography sx={{ fontWeight: 500 }}>
                          {tx.description || 'Transfer'}
                        </Typography>
                        <Typography variant="body2" color="text.secondary">
                          {new Date(tx.createdAt).toLocaleString()}
                        </Typography>
                      </Box>

                      <Typography
                        sx={{
                          fontWeight: 600,
                          color: tx.sign === '+' ? 'success.main' : 'error.main'
                        }}
                      >
                        {tx.sign}₪{tx.amount.toLocaleString()}
                      </Typography>
                    </Stack>
                  ))}
                </Stack>
              )}
            </Paper>
          </Grid>

          {/* ===== Quick Actions ===== */}
          <Grid item xs={12} md={5}>
            <Paper sx={{ p: 3, height: '100%' }}>
              <Typography variant="h6" sx={{ mb: 2, fontWeight: 600 }}>
                Quick actions
              </Typography>
              <Stack spacing={2}>
                <Button
                  variant="outlined"
                  startIcon={<SwapHorizIcon />}
                  onClick={() => navigate('/transfer')}
                >
                  New transfer
                </Button>
                <Button variant="outlined" startIcon={<AddCardIcon />} disabled>
                  Add a new card
                </Button>
                <Button variant="outlined" startIcon={<SavingsIcon />} disabled>
                  Top up savings
                </Button>
              </Stack>
            </Paper>
          </Grid>
        </Grid>
      </Container>
    </Box>
  );
}
