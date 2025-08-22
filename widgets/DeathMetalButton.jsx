// import * as Juce from "../juce/index.js"
import * as Juce from "juce-framework-frontend";
import React, {useEffect, useState} from "react"
import "./styles.css"

function DeathMetalButton({parameterName, children}){

    const toggleState = Juce.getToggleState(parameterName);
    const [parameter, setParameter] = useState(toggleState.getValue());
    
    function handleBypass(){
        toggleState.setValue(!toggleState.getValue());
    }

    useEffect(() => {
        const parameterListenerId = toggleState.valueChangedEvent.addListener(() => {
            setParameter(!toggleState.getValue());
        }, []);
        return () => toggleState.valueChangedEvent.removeListener(parameterListenerId);
    });

    return(
        <>
            <h1>Hello DeathMetalButton {parameterName}</h1>
            <button className={`death-metal-button ${parameter ? "active": "bypassed"}`} onClick={handleBypass}>{children.toUpperCase()}</button>
        </>
    );
}

export default DeathMetalButton;