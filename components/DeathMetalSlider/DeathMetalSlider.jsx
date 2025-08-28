import React, { useState, useEffect, useLayoutEffect, useRef } from "react";
import * as Juce from "juce-framework-frontend";

function DeathMetalSlider({ parameterName, width = 200, height = 24 }) {
    const canvasRef = useRef(null);
    const sliderState = Juce.getSliderState(parameterName);
    const [parameter, setParameter] = useState(sliderState.getNormalisedValue());

    const pad = 10; // matches your drawing

    // Keep React state in sync with JUCE
    useEffect(() => {
        const id = sliderState.valueChangedEvent.addListener(() => {
            setParameter(sliderState.getNormalisedValue());
        });
        return () => sliderState.valueChangedEvent.removeListener(id);
    }, [sliderState]);

    // Draw using CSS-pixel coordinates (DPR aware)
    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) return;

        const dpr = window.devicePixelRatio || 1;
        // CSS size
        canvas.style.width = `${width}px`;
        canvas.style.height = `${height}px`;
        // Buffer size
        canvas.width = Math.round(width * dpr);
        canvas.height = Math.round(height * dpr);

        const ctx = canvas.getContext("2d");
        ctx.setTransform(dpr, 0, 0, dpr, 0, 0); // 1 unit == 1 CSS px

        // background
        ctx.clearRect(0, 0, width, height);
        ctx.fillStyle = "black";
        ctx.fillRect(0, 0, width, height);

        // track
        const centerY = height / 2;
        ctx.lineWidth = 10;
        ctx.lineCap = "round";

        ctx.beginPath();
        ctx.strokeStyle = "grey";
        ctx.moveTo(pad, centerY);
        ctx.lineTo(width - pad, centerY);
        ctx.stroke();

        // value line
        const currentValue = pad + (width - 2 * pad) * parameter; // CSS px
        ctx.beginPath();
        ctx.strokeStyle = "white";
        ctx.shadowColor = "yellow";
        ctx.shadowBlur = 5;
        ctx.moveTo(pad, centerY);
        ctx.lineTo(currentValue, centerY);
        ctx.stroke();
        ctx.shadowBlur = 0;

        //         // Pentagram
        ctx.save();
        ctx.translate(currentValue, centerY);
        ctx.rotate(45);
        ctx.beginPath();

        ctx.fillStyle = "black";
        ctx.arc(0, 0, height / 2 - 1, 0, Math.PI * 2);
        ctx.fill();

        ctx.beginPath();
        for (let i = 0; i < 5; ++i) {
            const angle = (Math.PI / 180) * (180 + i * 144);
            const px = Math.cos(angle) * 10;
            const py = Math.sin(angle) * 10;

            i === 0 ? ctx.moveTo(px, py) : ctx.lineTo(px, py);
        }
        ctx.closePath();

        ctx.strokeStyle = "white";
        ctx.shadowBlur = 0;
        ctx.lineCap = "butt";
        ctx.lineWidth = 2;
        ctx.stroke();

        ctx.beginPath();
        ctx.arc(0, 0, 10 * 0.9, 0, Math.PI * 2);
        ctx.stroke();
        ctx.restore();



    });

    const handleMouseDown = (e) => {
        const el = e.currentTarget; // canvas element
        const x = e.nativeEvent.offsetX; // CSS px inside canvas

        const span = Math.max(1, width - 2 * pad);
        let t = (x - pad) / span;            // map to [0..1]
        t = Math.max(0, Math.min(1, t));     // clamp

        // start drag
        sliderState.sliderDragStarted?.();
        sliderState.setNormalisedValue(t);
        setParameter(t); // immediate visual update

        // For exactness: this will equal the click position (within fp error)
        // console.log("offsetX", x, "drawX", pad + span * t);

        // drag move
        const rect = el.getBoundingClientRect(); // measure once
        const onMove = (ev) => {
            const xCss = ev.clientX - rect.left;   // CSS px
            let nt = (xCss - pad) / span;
            nt = Math.max(0, Math.min(1, nt));
            sliderState.setNormalisedValue(nt);
            setParameter(nt);
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
        let deltaY = e.deltaY * 0.1;
        let paramCurrent = sliderState.getNormalisedValue();

        paramCurrent = Math.max(0.0, Math.min(1.0, paramCurrent - deltaY * 0.005));

        sliderState.sliderDragStarted();
        sliderState.setNormalisedValue(paramCurrent);
        sliderState.sliderDragEnded();
    }

    return (
        <>
            <canvas ref={canvasRef} onMouseDown={handleMouseDown} onWheel={handleWheel} />
        </>
    );
}

export default DeathMetalSlider;