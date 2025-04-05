import * as Juce from "./juce/index.js";

let bypass = document.querySelector(".bypass");
let parameterName = bypass.getAttribute("parameter");

if (!parameterName) {
  console.error("Missing data-parameter attribute on knob");
}

const toggleState = Juce.getToggleState(parameterName);

bypass.addEventListener("click", (e) => {
  toggleState.setValue(!toggleState.getValue());
  bypass.style.color = toggleState.getValue() ? "white" : "grey";
});

const nativeFunction = Juce.getNativeFunction("nativeFunction");

const data = window.__JUCE__.initialisationData;

if (data.isStandalone[0]) {
  let settingsButton = document.createElement("button");
  settingsButton.setAttribute("class", "settings");
  settingsButton.textContent = "";
  settingsButton.style.position = "fixed";
  settingsButton.style.bottom = "10px";
  settingsButton.style.right = "10px";
  settingsButton.style.fontFamily = "WebFont";
  settingsButton.style.fontSize = "30px";
  settingsButton.style.backgroundColor = "black";
  settingsButton.style.color = "white";
  settingsButton.style.border = "none";
  settingsButton.style.borderRadius = "5px";

  settingsButton.addEventListener("click", () => {
    nativeFunction("one", 2, null).then((result) => {});
  });
  document.body.appendChild(settingsButton);
}
