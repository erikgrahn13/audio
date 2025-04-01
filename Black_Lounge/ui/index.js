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
