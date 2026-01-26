// Pending
const pendingPromise = new Promise(() => {});

// Fulfilled
const fulfilledPromise = new Promise((resolve, reject) => {
  resolve("Success!");
});

fulfilledPromise.then(value => console.log("Fulfilled:", value));

// Rejected
const rejectedPromise = new Promise((resolve, reject) => {
  reject("Error!");
});

rejectedPromise.catch(error => console.log("Rejected:", error));

