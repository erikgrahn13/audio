import * as Juce from "../../external/juce/modules/juce_gui_extra/native/javascript/index.js";

class DeathMetalButton {
  constructor(deathMetalButton, options = {}) {
    if (!(deathMetalButton instanceof HTMLButtonElement)) {
      console.error("Invalid button element provided");
      return;
    }

    this.button = deathMetalButton;
    this.parameterName = this.button.dataset.parameter;
    if (!this.parameterName) {
      console.error("Missing data-parameter attribute on button");
      return;
    }

    const loadCustomFont = async () => {
      const font = new FontFace(
        "DeathMetalFont",
        'url(../../resources/fonts/ArtDystopia.ttf) format("truetype")'
      );

      await font.load();
      document.fonts.add(font);
      this.button.style.fontFamily = "'DeathMetalFont', sans-serif";
    };

    this.button.textContent = this.button.textContent.toUpperCase();
    this.button.style.textAlign = "center";
    this.button.style.borderRadius = "10px";
    this.button.style.padding = "10px 10px";
    // this.button.style.lineHeight = this.button.style.height;
    this.button.style.verticalAlign = "middle";
    this.button.style.backgroundColor = "black";
    // this.button.style.display = "flex";
    this.button.style.color = "white";
    loadCustomFont();

    this.button.addEventListener("click", (event) => {
      console.log(this.button.textContent + "clicked");
    });
  }
}

document.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll(".DeathMetalButton").forEach((button) => {
    new DeathMetalButton(button, {});
  });
});
