let myButton = document.querySelector(".myButton");
let myTitle = document.querySelector(".title");

const canvas = document.querySelector("#canvas");
const ctx = canvas.getContext("2d");

ctx.strokeStyle = "black";
ctx.beginPath();
ctx.lineWidth = 2;
ctx.arc(
  canvas.width / 2,
  canvas.height / 2,
  canvas.height / 2,
  0,
  Math.PI * 2,
  true
);
ctx.stroke();
// ctx.moveTo(110, 75)

// canvas.addEventListener("mouseover", (event) => {
//   ctx.fillStyle = "green";
//   ctx.fillRect(10, 10, 200, 100);
// });

// canvas.addEventListener("mouseout", (event) => {
//   ctx.fillStyle = "red";
//   ctx.fillRect(10, 10, 200, 100);
// });

myButton.addEventListener("click", (e) => {
  console.log("Button clicked");
  myTitle.style.color = "red";
  myTitle.textContent = "Hej";
});
