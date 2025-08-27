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

const noteNames = [
  "C",
  "C#",
  "D",
  "D#",
  "E",
  "F",
  "F#",
  "G",
  "G#",
  "A",
  "A#",
  "B",
];

let tuner = document.querySelector("#tuner");
let smoothedCents = 0;
let stableTime = 0; // seconds the pitch has been inside the tolerance
const requiredStableTime = 0.2; // 400 ms to be considered "locked in"
let lastUpdateTime = performance.now(); // for frame timing

window.__JUCE__.backend.addEventListener("pitch", (pitchHz) => {
  if (pitchHz <= 0) return;

  const currentTime = performance.now();
  const deltaTime = (currentTime - lastUpdateTime) / 1000.0; // seconds since last frame
  lastUpdateTime = currentTime;

  const A4 = 440;
  const midiNoteFloat = 69 + 12 * Math.log2(pitchHz / A4);
  const midiNote = Math.round(midiNoteFloat);
  const cents = (midiNoteFloat - midiNote) * 100;

  const note = noteNames[midiNote % 12];
  const octave = Math.floor(midiNote / 12) - 1;

  // Smoothing
  const smoothingFactor = 0.9;
  smoothedCents =
    smoothingFactor * smoothedCents + (1 - smoothingFactor) * cents;

  const roundedCents = Math.round(smoothedCents);

  const tuningTolerance = 2; // ±2 cents is acceptable

  // Update stable time counter
  if (Math.abs(roundedCents) <= tuningTolerance) {
    stableTime += deltaTime;
  } else {
    stableTime = 0; // reset if out of tune
  }

  // Show depending on stable time
  if (stableTime >= requiredStableTime) {
    tuner.textContent = `> ${note} <`; // Now really locked in
    // tuner.style.color = "limegreen";
  } else if (roundedCents > tuningTolerance) {
    tuner.textContent = `${note} <`; // too sharp
    // tuner.style.color = "white";
  } else if (roundedCents < -tuningTolerance) {
    tuner.textContent = `> ${note}`; // too flat
    // tuner.style.color = "white";
  }
});

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
