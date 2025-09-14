import * as Juce from "juce-framework-frontend";
import React, { useEffect, useState, useRef } from "react"
import styles from "./DeathMetalSwitch.module.css"

function DeathMetalSwitch({ parameterName, orientation = "horizontal", style = {}, children }) {

    const canvasRef = useRef(null);
    const toggleState = Juce.getToggleState(parameterName);
    const [parameter, setParameter] = useState(toggleState.getValue());

    function handleBypass() {
        toggleState.setValue(!toggleState.getValue());
    }

    useEffect(() => {
        const parameterListenerId = toggleState.valueChangedEvent.addListener(() => {
            setParameter(toggleState.getValue());
        }, []);
        return () => toggleState.valueChangedEvent.removeListener(parameterListenerId);
    }, [toggleState]);

    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) return;

        const dpr = window.devicePixelRatio || 1;
        // CSS size
        const size = 22;

        canvas.style.width = `${size}px`;
        canvas.style.height = `${size}px`;
        // Buffer size
        canvas.width = Math.round(size * dpr);
        canvas.height = Math.round(size * dpr);

        const ctx = canvas.getContext("2d");
        ctx.setTransform(dpr, 0, 0, dpr, 0, 0); // 1 unit == 1 CSS px



        // Pentagram
        const centerY = size / 2;
        ctx.save();
        ctx.translate(size / 2, centerY);
        ctx.rotate(45);
        ctx.beginPath();

        ctx.fillStyle = "black";
        // ctx.arc(0, 0, 10, 0, Math.PI * 2);
        ctx.arc(0, 0, size / 2, 0, Math.PI * 2);

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

    return (
        <>
            <button
                className={styles.switch}
                data-orientation={orientation}
                type="button"
                data-checked={parameter}
                onClick={handleBypass}
                style={style}
            >
                <span className={styles.track}></span>
                <canvas ref={canvasRef} className={styles.thumb}></canvas>
            </button>
        </>
    );
}

export default DeathMetalSwitch;