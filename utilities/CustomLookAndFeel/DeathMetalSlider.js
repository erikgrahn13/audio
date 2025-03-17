import * as Juce from "../../external/juce/modules/juce_gui_extra/native/javascript/index.js";

class DeathMetalSlider extends HTMLElement {
  constructor(deathMetalSlider, options = {}) {
    super();
    this.attachShadow({ mode: "open" });

    this.slider = deathMetalSlider;
    this.parameterName = this.getAttribute("parameter");
    const sliderState = Juce.getSliderState(this.parameterName);

    console.log(this.parameterName);
    if (!this.parameterName) {
      console.error("Missing data-parameter attribute on slider");
      return;
    }
    this.isDragging = false;

    this.canvas = document.createElement("canvas");
    this.canvas.height = 20;
    this.centerX = this.canvas.width * 0.5;
    this.centerY = this.canvas.height * 0.5;
    this.minValue = 0;
    this.maxValue = 100;
    this.value = 50;

    this.ctx = this.canvas.getContext("2d");

    this.shadowRoot.appendChild(this.canvas);
    this.draw();

    sliderState.valueChangedEvent.addListener(() => {
      this.canvas.value = sliderState.getNormalisedValue();
      this.value = this.canvas.value * 100;
      this.draw();
    });

    this.canvas.addEventListener("mousedown", (e) => {
      sliderState.sliderDragStarted();
      const rect = this.canvas.getBoundingClientRect();
      const mouseX = e.clientX;
      const mouseY = e.clientY;

      if (
        mouseX - rect.left >= this.value - 5 &&
        mouseX - rect.left <= this.value + 5 &&
        mouseY - rect.top >= this.centerY - 5 &&
        mouseY - rect.top <= this.centerY + 5
      ) {
        this.isDragging = true;
        this.lastMouseX = e.clientX;
      } else {
        this.value = Math.max(
          15,
          Math.min(this.canvas.width - 15, mouseX - rect.left)
        );

        let paramValue = ((this.value - 15) / (285 - 15)) * 100;
        sliderState.setNormalisedValue(paramValue / 100);

        // TODO: Handle param value to plugin here
        this.draw();
        this.isDragging = true;
        this.lastMouseX = e.clientX;
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
        this.value = 15;
      } else if (e.clientX > rect.right) {
        this.value = this.canvas.width - 15;
      } else {
        let deltaX = e.clientX - this.lastMouseX;
        this.lastMouseX = e.clientX;

        this.value = Math.max(
          15,
          Math.min(this.canvas.width - 15, this.value + deltaX)
        );
      }

      // TODO: Handle param value to plugin here
      let paramValue = ((this.value - 15) / (285 - 15)) * 100;
      sliderState.setNormalisedValue(paramValue / 100);

      this.draw();
    });

    this.canvas.addEventListener("wheel", (e) => {
      let deltaX = e.deltaY * 0.1;

      this.value = Math.max(
        15,
        Math.min(this.canvas.width - 15, this.value - deltaX)
      );

      let paramValue = ((this.value - 15) / (285 - 15)) * 100;
      sliderState.setNormalisedValue(paramValue / 100);
      this.draw();
    });
  }

  isMouseOverThumb(mouseX, mouseY) {
    const rect = this.canvas.getBoundingClientRect();
    const relativeX = mouseX - rect.left;
    const relativeY = mouseY - rect.top;

    console.log(rect);

    // Get current pentagram position
    const trackWidth = this.canvas.width - this.thumbSize;
    const thumbX =
      ((this.value - this.minValue) / (this.maxValue - this.minValue)) *
      trackWidth;
    const thumbY = this.canvas.height / 2;

    // Define a rectangular hitbox around the pentagram
    const hitboxWidth = this.thumbSize * 1.2; // Slightly larger for easier clicking
    const hitboxHeight = this.thumbSize * 1.2;
    const hitboxX = thumbX - hitboxWidth / 2;
    const hitboxY = thumbY - hitboxHeight / 2;

    // Check if the mouse is inside the rectangular bounds
    return (
      relativeX >= hitboxX &&
      relativeX <= hitboxX + hitboxWidth &&
      relativeY >= hitboxY &&
      relativeY <= hitboxY + hitboxHeight
    );
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

    // this.value = (this.value * this.canvas.width) / this.maxValue;

    // Draw the white line
    this.ctx.beginPath();
    this.ctx.moveTo(10, this.centerY);
    this.ctx.lineTo(this.value, this.centerY);
    this.ctx.lineWidth = 10;
    this.ctx.strokeStyle = "white";
    this.ctx.lineCap = "round";
    this.ctx.stroke();

    this.drawPentagram(this.value, this.centerY, 10, 45);
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
