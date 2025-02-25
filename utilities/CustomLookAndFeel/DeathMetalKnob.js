import * as Juce from "../../external/juce/modules/juce_gui_extra/native/javascript/index.js";

class DeathMetalKnob {
  constructor(canvasElement, options = {}) {
    if (!(canvasElement instanceof HTMLCanvasElement)) {
      console.error("Invalid canvas element provided");
      return;
    }

    this.canvas = canvasElement;
    this.canvas = canvasElement;
    this.parameterName = this.canvas.dataset.parameter;
    if (!this.parameterName) {
      console.error("Missing data-parameter attribute on canvas");
      return;
    }

    this.ctx = this.canvas.getContext("2d");

    const sliderState = Juce.getSliderState(this.parameterName);
    this.isDragging = false;
    this.radius = this.canvas.height * 0.5 - 4;
    this.centerX = this.canvas.width * 0.5;
    this.centerY = this.canvas.height * 0.5;
    this.startAngleRadians = Math.PI * 1.2 - Math.PI / 2;
    this.endAngleRadians = Math.PI * 2.8 - Math.PI / 2;
    this.angle = (this.startAngleRadians + this.endAngleRadians) / 2;
    this.minValue = 0;
    this.maxValue = 100;
    this.value = 50;

    this.lastMouseY = 0;

    sliderState.valueChangedEvent.addListener(() => {
      this.canvas.value = sliderState.getNormalisedValue();
      this.value = this.canvas.value * 100;
      this.draw();
    });

    this.canvas.addEventListener("mousedown", (e) => {
      sliderState.sliderDragStarted();
      this.isDragging = true;
      this.lastMouseY = e.clientY;
    });

    document.addEventListener("mousemove", (e) => {
      if (!this.isDragging) return;

      let deltaY = e.clientY - this.lastMouseY;
      this.lastMouseY = e.clientY;

      this.value = Math.max(
        this.minValue,
        Math.min(this.maxValue, this.value - deltaY * 0.25)
      );
      sliderState.setNormalisedValue(this.value / 100);
      this.angle = this.valueToAngle(this.value);
      this.draw();
    });

    document.addEventListener("mouseup", () => {
      if (this.isDragging) {
        sliderState.sliderDragEnded();
        this.isDragging = false;
      }
    });

    this.draw();
  }

  draw() {
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    this.ctx.save();

    // Draw main knob circle
    this.ctx.beginPath();
    this.ctx.arc(this.centerX, this.centerY, this.radius, 0, Math.PI * 2, true);
    this.ctx.fillStyle = "black";
    this.ctx.fill();

    let lineW = this.radius * 0.135;
    this.ctx.lineWidth = lineW;
    let adjustedRadius = this.radius - lineW * 0.5;
    this.ctx.lineCap = "round";

    // Draw background arc (full range)
    this.ctx.strokeStyle = "grey";
    this.ctx.beginPath();
    this.ctx.arc(
      this.centerX,
      this.centerY,
      adjustedRadius,
      this.startAngleRadians,
      this.endAngleRadians,
      false
    );
    this.ctx.stroke();

    // 🎯 Compute the angle from the value
    let currentAngle = this.valueToAngle(this.value);

    // Draw the filled portion (indicating value)
    this.ctx.strokeStyle = "white";
    this.ctx.shadowColor = "yellow";
    this.ctx.shadowBlur = 5;
    this.ctx.beginPath();
    this.ctx.arc(
      this.centerX,
      this.centerY,
      adjustedRadius,
      this.startAngleRadians,
      currentAngle,
      false
    );
    this.ctx.stroke();

    this.drawPentagram(
      this.centerX,
      this.centerY,
      this.radius * 0.8,
      currentAngle
    );
    this.drawCross(this.centerX, this.centerY, this.radius, currentAngle);
    this.ctx.restore();
  }

  drawPentagram(x, y, size, rotation) {
    this.ctx.save();
    this.ctx.translate(x, y);
    this.ctx.rotate(rotation);
    this.ctx.beginPath();

    for (let i = 0; i < 5; i++) {
      const angle = (Math.PI / 180) * (180 + i * 144);
      const px = Math.cos(angle) * size;
      const py = Math.sin(angle) * size;

      if (i === 0) {
        this.ctx.moveTo(px, py);
      } else {
        this.ctx.lineTo(px, py);
      }
    }
    this.ctx.closePath();

    this.ctx.strokeStyle = "white";
    this.ctx.shadowBlur = 0;
    this.ctx.lineCap = "butt";
    this.ctx.lineWidth = 2;
    this.ctx.stroke();

    // Draw inner circle
    this.ctx.beginPath();
    this.ctx.arc(0, 0, size * 0.9, 0, Math.PI * 2);
    this.ctx.stroke();
    this.ctx.restore();
  }

  drawCross(x, y, size, rotation) {
    this.ctx.save();
    this.ctx.translate(x, y);
    this.ctx.rotate(rotation);
    this.ctx.beginPath();
    this.ctx.shadowBlur = 0;
    this.ctx.lineCap = "butt";

    this.ctx.moveTo(0, 0);
    this.ctx.lineTo(size * 1.1, 0);
    this.ctx.stroke();

    this.ctx.moveTo(size / 3, -size / 4);
    this.ctx.lineTo(size / 3, size / 4);
    this.ctx.stroke();

    this.ctx.restore();
  }

  valueToAngle(val) {
    return (
      this.startAngleRadians +
      ((val - this.minValue) / (this.maxValue - this.minValue)) *
        (this.endAngleRadians - this.startAngleRadians)
    );
  }
}

document.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll(".DeathMetalKnob").forEach((canvas) => {
    new DeathMetalKnob(canvas, {});
  });
});
