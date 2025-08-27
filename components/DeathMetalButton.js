import * as Juce from "../juce/index.js";

class DeathMetalButton extends HTMLElement {
  constructor(options = {}) {
    super();
    this.attachShadow({ mode: "open" });

    this.button = document.createElement("button");

    this.parameterName = this.getAttribute("parameter");
    this.parameterValue;
    const toggleState = Juce.getToggleState(this.parameterName);

    if (!this.parameterName) {
      console.error("Missing data-parameter attribute on button");
      return;
    }

    const loadCustomFont = async () => {
      const font = new FontFace(
        "DeathMetalFont",
        'url(../../../resources/fonts/ArtDystopia.ttf) format("truetype")'
      );

      await font.load();
      document.fonts.add(font);
      this.button.style.fontFamily = "'DeathMetalFont', sans-serif";
    };

    toggleState.valueChangedEvent.addListener(() => {
      this.parameterValue = toggleState.getValue();
    });

    this.button.textContent = this.textContent.toUpperCase();
    this.button.style.textAlign = "center";
    this.button.style.borderRadius = "10px";
    this.button.style.padding = "10px 10px";
    // this.button.style.lineHeight = this.button.style.height;
    this.button.style.verticalAlign = "middle";
    this.button.style.backgroundColor = "black";
    // this.button.style.display = "flex";
    this.button.style.color = "white";
    loadCustomFont();

    this.shadowRoot.appendChild(this.button);

    this.button.addEventListener("click", (event) => {});
  }
}

customElements.define("death-metal-button", DeathMetalButton);
