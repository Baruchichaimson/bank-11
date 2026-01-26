self.addEventListener("message", (event) => {
  if (event.data === "ping") 
  {
    setTimeout(() => {self.postMessage("pong");}, 4000);
  }
});
