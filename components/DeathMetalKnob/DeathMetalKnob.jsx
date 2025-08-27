import React, { useState, useEffect, useRef } from "react"
import * as Juce from "juce-framework-frontend";

function DeathMetalKnob({ parameterName, size }) {

  const canvasRef = useRef(null);
  const sliderState = Juce.getSliderState(parameterName);
  const [parameter, setParameter] = useState(sliderState.getNormalisedValue());

  const startAngleRadians = Math.PI * 1.2 - Math.PI / 2;
  const endAngleRadians = Math.PI * 2.8 - Math.PI / 2;

  const isDraggingRef = useRef(false);

  const handleChange = (e) => {
    sliderState.sliderDragStarted();
    isDraggingRef.current = true;
    let lastY = e.clientY;

    let paramCurrent = sliderState.getNormalisedValue();

    const onMouseMove = (e) => {
      if (!isDraggingRef.current) return;

      const deltaY = e.clientY - lastY;
      lastY = e.clientY;
      paramCurrent = Math.max(0.0, Math.min(1.0, paramCurrent - deltaY * 0.0025));
      sliderState.setNormalisedValue(paramCurrent);
    };

    const onMouseUp = (e) => {
      if (!isDraggingRef.current) return;
      sliderState.sliderDragEnded();
      isDraggingRef.current = false;

      document.removeEventListener("mousemove", onMouseMove);
    };

    document.addEventListener("mousemove", onMouseMove);
    document.addEventListener("mouseup", onMouseUp, { once: true });

  };

  const handleWheel = (e) => {
    let deltaY = e.deltaY * 0.1;
    let paramCurrent = sliderState.getNormalisedValue();

    paramCurrent = Math.max(0.0, Math.min(1.0, paramCurrent - deltaY * 0.005));

    sliderState.sliderDragStarted();
    sliderState.setNormalisedValue(paramCurrent);
    sliderState.sliderDragEnded();
  }

  useEffect(() => {
    const valueListenerId = sliderState.valueChangedEvent.addListener(() => {
      setParameter(sliderState.getNormalisedValue());
    });
    // const propertiesListenerId = sliderState.propertiesChangedEvent.addListener(
    //   () => setProperties(sliderState.properties)
    // );

    return function cleanup() {
      sliderState.valueChangedEvent.removeListener(valueListenerId);
      // sliderState.propertiesChangedEvent.removeListener(propertiesListenerId);
    };
  }, [sliderState]);








  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.save();

    canvas.width = size;
    canvas.height = size;
    const centerX = canvas.width * 0.5;
    const centerY = canvas.height * 0.5;
    const radius = canvas.height * 0.5 - 4;

    // Do your drawing here
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // Draw main knob circle
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2, true);
    ctx.fillStyle = "black";
    ctx.fill();

    const lineW = radius * 0.135;
    ctx.lineWidth = lineW;
    const adjustedRadius = radius - lineW * 0.5;
    ctx.lineCap = "round";

    // Draw background arc (full range)
    ctx.strokeStyle = "grey";
    ctx.beginPath();
    ctx.arc(
      centerX,
      centerY,
      adjustedRadius,
      startAngleRadians,
      endAngleRadians,
      false
    );
    ctx.stroke();

    let currentAngle = valueToAngle(parameter);
    // Draw the filled portion (indicating value)
    ctx.strokeStyle = "white";
    ctx.shadowColor = "yellow";
    ctx.shadowBlur = 5;
    ctx.beginPath();
    ctx.arc(
      centerX,
      centerY,
      adjustedRadius,
      startAngleRadians,
      currentAngle,
      false
    );
    ctx.stroke();

    // Draw pentagram
    ctx.save();
    ctx.translate(centerX, centerY);
    ctx.rotate(currentAngle);
    ctx.beginPath();

    for (let i = 0; i < 5; i++) {
      const angle = (Math.PI / 180) * (180 + i * 144);
      const px = Math.cos(angle) * (radius * 0.8);
      const py = Math.sin(angle) * (radius * 0.8);

      if (i === 0) {
        ctx.moveTo(px, py);
      } else {
        ctx.lineTo(px, py);
      }
    }
    ctx.closePath();

    ctx.strokeStyle = "white";
    ctx.shadowBlur = 0;
    ctx.lineCap = "butt";
    ctx.lineWidth = 2;
    ctx.stroke();

    // Draw inner circle
    ctx.beginPath();
    ctx.arc(0, 0, (radius * 0.8) * 0.9, 0, Math.PI * 2);
    ctx.stroke();
    ctx.restore();


    // Draw cross
    ctx.save();
    ctx.translate(centerX, centerY);
    ctx.rotate(currentAngle);
    ctx.beginPath();
    ctx.shadowBlur = 0;
    ctx.lineCap = "butt"
    ctx.moveTo(0, 0);
    ctx.lineTo(radius * 1.1, 0);
    ctx.stroke();
    ctx.moveTo(radius / 3, -radius / 4);
    ctx.lineTo(radius / 3, radius / 4);
    ctx.stroke();
    ctx.restore();
  });

  function valueToAngle(val) {
    return (startAngleRadians + ((val - 0.0) / (1.0 - 0.0)) * (endAngleRadians - startAngleRadians));
  }



  return (
    <canvas ref={canvasRef} width={100} height={100} onMouseDown={handleChange} onWheel={handleWheel}></canvas>
  );
}

export default DeathMetalKnob;