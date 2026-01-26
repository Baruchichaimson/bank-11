function fibonacci() 
{
  const memo = {};

  function fib(n) 
  {
    if (n <= 1) 
    {
        return n;
    }

    if (memo[n]) 
    {
        return memo[n];
    } 

    memo[n] = fib(n - 1) + fib(n - 2); 

    return memo[n];
  }

  return fib;
}

const fib = fibonacci();
console.log(fib(10)); 
console.log(fib(50)); 
