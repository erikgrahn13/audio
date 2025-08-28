// import { useState } from 'react'
// import reactLogo from './assets/react.svg'
// import viteLogo from '/vite.svg'
import hotDog from "./assets/hotdog.png";
import chainsaws from "./assets/chainsaws.png"
import * as Juce from "juce-framework-frontend";


import './App.css'
import { DeathMetalKnob, DeathMetalSlider, DeathMetalSwitch } from "@abyss-lounge/components"
import { useState, useEffect } from "react";

const NOTE = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];

function App() {
  const data = window.__JUCE__.initialisationData;
  const nativeFunction = Juce.getNativeFunction("nativeFunction");
  const [pitch, setPitch] = useState("> - <");
  const handleSettings = (e) => {
    nativeFunction("one", 2, null).then((result) => { });
  }


  useEffect(() => {
    const backend = window.__JUCE__?.backend;
    if (!backend) return;

    // state that should persist across events but NOT re-create listeners
    let smoothedCents = 0;
    let stableTime = 0;
    const requiredStableTime = 0.2;      // seconds
    const tolerance = 2;                  // ± cents
    let lastUpdate = performance.now();

    const handler = (pitchHz) => {
      if (pitchHz <= 0) return;

      const now = performance.now();
      const dt = (now - lastUpdate) / 1000;
      lastUpdate = now;

      const A4 = 440;
      const noteFloat = 69 + 12 * Math.log2(pitchHz / A4);
      const noteIdx = Math.round(noteFloat);
      const cents = (noteFloat - noteIdx) * 100;

      const noteName = NOTE[(noteIdx % 12 + 12) % 12]; // safe mod

      // smooth cents
      const alpha = 0.9;
      smoothedCents = alpha * smoothedCents + (1 - alpha) * cents;
      const r = Math.round(smoothedCents);

      if (Math.abs(r) <= tolerance) stableTime += dt;
      else stableTime = 0;

      if (stableTime >= requiredStableTime) setPitch(`> ${noteName} <`);
      else if (r > tolerance) setPitch(`${noteName} <`);
      else if (r < -tolerance) setPitch(`> ${noteName}`);
    };

    // subscribe once
    const subscription = backend.addEventListener("pitch", handler);

    // cleanup (covers different API shapes)
    return () => {
      if (typeof subscription === "function") subscription();                 // unsubscribe function
      else if (backend.removeEventListener) backend.removeEventListener("pitch", subscription ?? handler);
    };
  }, []); // run once (StrictMode will mount/unmount twice in dev; cleanup handles it)



  return (
    <>
      <h1 className="title">BLACK LOUNGE AMP</h1>
      <h1 id="tuner">{pitch}</h1>
      <div className="img-stack">
        <img src={chainsaws} height="250px" className="base"></img>
        <img src={hotDog} height="250px" className="overlay" />

      </div>
      <div className="knob-area">
        <div className="denoiser-area">
          <h2>DENOISER</h2>
          <span style={{ display: 'inline-flex', alignItems: 'center', gap: 20 }}>
            <DeathMetalSwitch className="death-metal-switch" parameterName="denoiserActive" size={20} orientation="vertical"></DeathMetalSwitch>
            <DeathMetalKnob parameterName="denoiser" size={60}></DeathMetalKnob>
          </span>
        </div>
        <div className="gain-area">
          <h2>GAIN</h2>
          <DeathMetalKnob parameterName="gain" size={60}></DeathMetalKnob>
        </div>
        <div className="volume-area">
          <h2>VOLUME</h2>
          <DeathMetalKnob parameterName="volume" size={60}></DeathMetalKnob>
        </div>

      </div >
      {data.isStandalone[0] && <button className="settings" onClick={handleSettings}></button>}
    </>
  )
}

export default App
