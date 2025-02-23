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
let angle = 0;

function drawKnob() {
  ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear before redrawing
  ctx.save(); // Save the current state
  ctx.translate(centerX, centerY); // Move to center
  ctx.rotate(angle); // Rotate canvas
  ctx.translate(-centerX, -centerY); // Move back

  ctx.beginPath();
  ctx.arc(centerX, centerY, radius, 0, Math.PI * 2, true);
  ctx.fill();

  let lineW = radius * 0.135;
  ctx.lineWidth = lineW;
  let adjustedRadius = radius - lineW * 0.5;

  ctx.lineCap = "round";
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

  ctx.strokeStyle = "white";
  ctx.shadowColor = "yellow";
  ctx.shadowBlur = "5";
  ctx.beginPath();
  ctx.arc(centerX, centerY, adjustedRadius, startAngleRadians, value, false);
  ctx.stroke();

  // Draw pentagram
  ctx.lineCap = "butt";
  ctx.shadowBlur = 0;
  ctx.strokeStyle = "white";
  ctx.lineWidth = "2";
  ctx.beginPath();
  for (let i = 0; i < 5; i++) {
    const angle = (Math.PI / 180) * (90 + i * 144); // Start at the top (-90°)
    const x = centerX + (radius - lineW * 1.5) * Math.cos(angle);
    const y = centerY + (radius - lineW * 1.5) * Math.sin(angle);

    if (i === 0) {
      ctx.moveTo(x, y);
    } else {
      ctx.lineTo(x, y);
    }
  }

  ctx.closePath();
  ctx.stroke();

  // Draw inner circle
  ctx.beginPath();
  ctx.arc(centerX, centerY, radius - lineW * 2, 0, Math.PI * 2);
  ctx.stroke();

  // Draw upside-down cross
  ctx.beginPath();
  ctx.lineWidth = lineW - 1;
  ctx.moveTo(centerX, centerY + radius / 8);
  ctx.lineTo(centerX, -radius);
  ctx.stroke();

  ctx.beginPath();
  ctx.moveTo(centerX - radius / 4, centerY - 10);
  ctx.lineTo(centerX + radius / 4, centerY - 10);
  ctx.stroke();
}

drawKnob();
// ctx.moveTo(centerX - lineW / 2, centerY + radius / 6);
// ctx.lineTo(centerX - lineW / 2 + lineW, centerY + radius / 6 - radius);

myButton.addEventListener("click", (e) => {
  console.log("Button clicked");
  myTitle.style.color = "red";
  myTitle.textContent = "Hej";
});

canvas.addEventListener("click", (event) => {
  console.log("Knob clicked");
  angle += (45 * Math.PI) / 180; // Rotate by 45 degrees
  drawKnob(); // Redraw with new rotation
});
