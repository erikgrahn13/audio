let myButton = document.querySelector(".myButton");
let myTitle = document.querySelector(".title");

const canvas = document.querySelector("#canvas");
const ctx = canvas.getContext("2d");

ctx.strokeStyle = "black";

let radius = canvas.height * 0.5 - 4;
let centerX = canvas.width * 0.5;
let centerY = canvas.height * 0.5;
let endAngleRadians = Math.PI * 0.2;
let startAngleRadians = Math.PI * 0.8;

let value = startAngleRadians + 2.2 * (endAngleRadians - startAngleRadians);

ctx.beginPath();
ctx.arc(centerX, centerY, radius, 0, Math.PI * 2, true);
ctx.fill();

let lineW = radius * 0.135;
ctx.lineWidth = lineW;
let adjustedRadius = radius - lineW * 0.5;

ctx.strokeStyle = "grey";
ctx.beginPath();
ctx.arc(
  centerX,
  centerY,
  adjustedRadius,
  startAngleRadians,
  endAngleRadians,
  false
);
ctx.stroke();

ctx.strokeStyle = "red";
ctx.beginPath();
ctx.arc(centerX, centerY, adjustedRadius, startAngleRadians, value, false);
ctx.stroke();

myButton.addEventListener("click", (e) => {
  console.log("Button clicked");
  myTitle.style.color = "red";
  myTitle.textContent = "Hej";
});
