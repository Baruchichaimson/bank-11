function logFunctionCall(fn) 
{
    return function(...args) 
    {
        console.log("Calling function with arguments:", args);
        const result = fn(...args);
        console.log("Function returned:", result);
        return result;
    };
}

function add(a, b) 
{
    return a + b;
}

const loggedAdd = logFunctionCall(add);

loggedAdd(2, 3);

