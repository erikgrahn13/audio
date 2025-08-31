import React, { useEffect, useRef, useState } from "react";
import * as Juce from "juce-framework-frontend";

function DeathMetalSlider({ parameterName, orientation = "horizontal", width = 200, height = 24 }) {
  const canvasRef = useRef(null);
  const sliderState = Juce.getSliderState(parameterName);
  const [parameter, setParameter] = useState(sliderState.getNormalisedValue());

  const pad = 10;

  // Keep in sync with JUCE
  useEffect(() => {
    const id = sliderState.valueChangedEvent.addListener(() => {
      setParameter(sliderState.getNormalisedValue());
    });
    return () => sliderState.valueChangedEvent.removeListener(id);
  }, [sliderState]);

  // Draw
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const dpr = window.devicePixelRatio || 1;
    canvas.style.width = `${width}px`;
    canvas.style.height = `${height}px`;
    canvas.width = Math.round(width * dpr);
    canvas.height = Math.round(height * dpr);

    const ctx = canvas.getContext("2d");
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);

    // bg
    ctx.clearRect(0, 0, width, height);
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, width, height);

    ctx.lineWidth = 10;
    ctx.lineCap = "round";

    if (orientation === "horizontal") {
      const cy = height / 2;
      const span = Math.max(1, width - 2 * pad);
      const xVal = pad + span * parameter;

      // track
      ctx.beginPath();
      ctx.strokeStyle = "grey";
      ctx.moveTo(pad, cy);
      ctx.lineTo(width - pad, cy);
      ctx.stroke();

      // value
      ctx.beginPath();
      ctx.strokeStyle = "white";
      ctx.shadowColor = "yellow";
      ctx.shadowBlur = 5;
      ctx.moveTo(pad, cy);
      ctx.lineTo(xVal, cy);
      ctx.stroke();
      ctx.shadowBlur = 0;

      // === Classic pentagram look (unchanged) ===
      const circleR = height / 2 - 1;      // same as your original
      const starR = 10;                     // same as your original
      const rot = 45 % (2 * Math.PI);       // reproduces your previous visual rotation (~58°)

      ctx.save();
      ctx.translate(xVal, cy);
      ctx.rotate(rot);

      // black circle
      ctx.beginPath();
      ctx.fillStyle = "black";
      ctx.arc(0, 0, circleR, 0, Math.PI * 2);
      ctx.fill();

      // star
      ctx.beginPath();
      for (let i = 0; i < 5; ++i) {
        const angle = (Math.PI / 180) * (180 + i * 144);
        const px = Math.cos(angle) * starR;
        const py = Math.sin(angle) * starR;
        i === 0 ? ctx.moveTo(px, py) : ctx.lineTo(px, py);
      }
      ctx.closePath();

      ctx.strokeStyle = "white";
      ctx.lineCap = "butt";
      ctx.lineWidth = 2;
      ctx.stroke();

      ctx.beginPath();
      ctx.arc(0, 0, starR * 0.9, 0, Math.PI * 2);
      ctx.stroke();

      ctx.restore();
    } else {
      // vertical: 0 at bottom → 1 at top
      const cx = width / 2;
      const span = Math.max(1, height - 2 * pad);
      const yVal = height - pad - span * parameter; // invert

      // track
      ctx.beginPath();
      ctx.strokeStyle = "grey";
      ctx.moveTo(cx, pad);
      ctx.lineTo(cx, height - pad);
      ctx.stroke();

      // value
      ctx.beginPath();
      ctx.strokeStyle = "white";
      ctx.shadowColor = "yellow";
      ctx.shadowBlur = 5;
      ctx.moveTo(cx, height - pad);
      ctx.lineTo(cx, yVal);
      ctx.stroke();
      ctx.shadowBlur = 0;

      // === Classic pentagram look (unchanged) ===
      const circleR = width / 2 - 1;        // note: use cross-axis (width) to keep same visual size
      const starR = 10;                      // same as your original
      const rot = 45 % (2 * Math.PI);        // same effective rotation as before

      ctx.save();
      ctx.translate(cx, yVal);
      ctx.rotate(rot);

      // black circle
      ctx.beginPath();
      ctx.fillStyle = "black";
      ctx.arc(0, 0, circleR, 0, Math.PI * 2);
      ctx.fill();

      // star
      ctx.beginPath();
      for (let i = 0; i < 5; ++i) {
        const angle = (Math.PI / 180) * (180 + i * 144);
        const px = Math.cos(angle) * starR;
        const py = Math.sin(angle) * starR;
        i === 0 ? ctx.moveTo(px, py) : ctx.lineTo(px, py);
      }
      ctx.closePath();

      ctx.strokeStyle = "white";
      ctx.lineCap = "butt";
      ctx.lineWidth = 2;
      ctx.stroke();

      ctx.beginPath();
      ctx.arc(0, 0, starR * 0.9, 0, Math.PI * 2);
      ctx.stroke();

      ctx.restore();
    }
  }, [width, height, parameter, orientation]);

  // Input
  const handleMouseDown = (e) => {
    const el = e.currentTarget;
    const rect = el.getBoundingClientRect();

    const toNorm = (clientX, clientY) => {
      if (orientation === "horizontal") {
        const span = Math.max(1, width - 2 * pad);
        const xCss = clientX - rect.left;
        return Math.max(0, Math.min(1, (xCss - pad) / span));
      } else {
        const span = Math.max(1, height - 2 * pad);
        const yCss = clientY - rect.top;
        return Math.max(0, Math.min(1, 1 - (yCss - pad) / span));
      }
    };

    const start = toNorm(e.clientX, e.clientY);
    sliderState.sliderDragStarted?.();
    sliderState.setNormalisedValue(start);
    setParameter(start);

    const onMove = (ev) => {
      const v = toNorm(ev.clientX, ev.clientY);
      sliderState.setNormalisedValue(v);
      setParameter(v);
    };
    const onUp = () => {
      sliderState.sliderDragEnded?.();
      window.removeEventListener("mousemove", onMove);
      window.removeEventListener("mouseup", onUp);
    };
    window.addEventListener("mousemove", onMove);
    window.addEventListener("mouseup", onUp, { once: true });
  };

  const handleWheel = (e) => {
    let v = sliderState.getNormalisedValue();
    v = Math.max(0, Math.min(1, v - Math.sign(e.deltaY) * 0.005));
    sliderState.sliderDragStarted?.();
    sliderState.setNormalisedValue(v);
    sliderState.sliderDragEnded?.();
  };

  return <canvas ref={canvasRef} onMouseDown={handleMouseDown} onWheel={handleWheel} style={{ cursor: "pointer" }} />;
}

export default DeathMetalSlider;
