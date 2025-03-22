import * as Juce from "../juce/index.js";

class DeathMetalSlider extends HTMLElement {
  constructor(deathMetalSlider, options = {}) {
    super();
    this.attachShadow({ mode: "open" });

    this.slider = deathMetalSlider;
    this.parameterName = this.getAttribute("parameter");
    const sliderState = Juce.getSliderState(this.parameterName);

    if (!this.parameterName) {
      console.error("Missing data-parameter attribute on slider");
      return;
    }
    this.isDragging = false;

    this.canvas = document.createElement("canvas");
    this.canvas.height = 20;
    this.centerX = this.canvas.width * 0.5;
    this.centerY = this.canvas.height * 0.5;
    this.minParameterValue = 0.0;
    this.maxParameterValue = 1.0;
    this.parameterValue = 0.5;

    this.ctx = this.canvas.getContext("2d");

    this.shadowRoot.appendChild(this.canvas);
    this.draw();

    sliderState.valueChangedEvent.addListener(() => {
      this.parameterValue = sliderState.getNormalisedValue();
      this.draw();
    });

    this.canvas.addEventListener("mousedown", (e) => {
      sliderState.sliderDragStarted();
      const rect = this.canvas.getBoundingClientRect();
      const mouseX = e.clientX;
      const mouseY = e.clientY;
      let xPosition = (this.parameterValue * 100 * (285 - 15)) / 100 + 15;

      if (
        mouseX - rect.left >= xPosition - 5 &&
        mouseX - rect.left <= xPosition + 5 &&
        mouseY - rect.top >= this.centerY - 5 &&
        mouseY - rect.top <= this.centerY + 5
      ) {
        this.isDragging = true;
        this.lastMouseX = e.clientX;
      } else {
        this.parameterValue = (mouseX - rect.left) / this.canvas.width;

        sliderState.setNormalisedValue(this.parameterValue);
        this.isDragging = true;
        this.lastMouseX = e.clientX;
        this.draw();
      }
    });

    document.addEventListener("mouseup", (e) => {
      if (this.isDragging) {
        sliderState.sliderDragEnded();
        this.isDragging = false;
      }
    });

    document.addEventListener("mousemove", (e) => {
      if (!this.isDragging) return;

      const rect = this.canvas.getBoundingClientRect();
      if (e.clientX < rect.left) {
        this.parameterValue = 0.0;
      } else if (e.clientX > rect.right) {
        this.parameterValue = 1.0;
      } else {
        let deltaX = e.clientX - this.lastMouseX;
        this.lastMouseX = e.clientX;

        let absolutePosition =
          (this.parameterValue * 100 * (285 - 15)) / 100 + 15;

        absolutePosition = Math.max(
          15,
          Math.min(this.canvas.width - 15, absolutePosition + deltaX)
        );

        this.parameterValue = (absolutePosition - 15) / (285 - 15);
      }

      sliderState.setNormalisedValue(this.parameterValue);
      this.draw();
    });

    this.canvas.addEventListener("wheel", (e) => {
      let deltaX = e.deltaY * 0.1;

      let absolutePosition =
        (this.parameterValue * 100 * (285 - 15)) / 100 + 15;

      absolutePosition = Math.max(
        15,
        Math.min(this.canvas.width - 15, absolutePosition - deltaX)
      );
      this.parameterValue = (absolutePosition - 15) / (285 - 15);

      sliderState.setNormalisedValue(this.parameterValue);
      this.draw();
    });
  }

  draw() {
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

    this.ctx.fillStyle = "black";
    this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

    this.ctx.beginPath();
    this.ctx.moveTo(10, this.centerY);
    this.ctx.lineTo(this.canvas.width - 10, this.centerY);
    this.ctx.lineWidth = 10;
    this.ctx.strokeStyle = "grey";
    this.ctx.lineCap = "round";
    this.ctx.stroke();

    let xPosition = (this.parameterValue * 100 * (285 - 15)) / 100 + 15;

    // Draw the white line

    this.ctx.beginPath();
    this.ctx.moveTo(10, this.centerY);
    this.ctx.lineTo(xPosition, this.centerY);
    this.ctx.lineWidth = 10;
    this.ctx.strokeStyle = "white";
    this.ctx.lineCap = "round";
    this.ctx.stroke();

    this.drawPentagram(xPosition, this.centerY, 10, 45);
  }

  drawPentagram(x, y, size, rotation) {
    this.ctx.save();
    this.ctx.translate(x, y);
    this.ctx.rotate(rotation);
    this.ctx.beginPath();

    // Draw background black arc
    this.ctx.fillStyle = "black";
    this.ctx.arc(0, 0, size, 0, Math.PI * 2);
    this.ctx.fill();

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
}

// Define the custom element
customElements.define("death-metal-slider", DeathMetalSlider);
