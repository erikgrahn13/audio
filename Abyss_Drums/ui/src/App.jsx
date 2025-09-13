import './App.css'
import DrumTypes from "./components/DrymType/DrumType";
import DrumSample from './components/DrumSample/DrumSample';
import Slots from "./components/Slots/Slots"
import Droppable from './components/Droppable/Droppable';
import React, {useState, useEffect} from 'react';
import * as Juce from "juce-framework-frontend";



import { DndContext, PointerSensor, useSensor, useSensors, DragOverlay, defaultDropAnimationSideEffects } from "@dnd-kit/core"

const data = window.__JUCE__.initialisationData;
const openSettingsView = Juce.getNativeFunction("openSettingsView");
const loadDrumSample = Juce.getNativeFunction("loadDrumSample");

// Get sample arrays
const kickSamples = data?.kickSamples?.[0] || [];
const snareSamples = data?.snareSamples?.[0] || [];

function App() {

  const handleSettings = () => {
    openSettingsView("one", 2, null);
  }

  const [activeId, setActiveId] = useState(null);
  const [activeLabel, setActiveLabel] = useState("");
  const [loadedSample1, setLoadedSample1] = useState(() => {
    try {
      const saved = sessionStorage.getItem('slot1');
      if (saved) {
        return JSON.parse(saved);
      }
    } catch (error) {
      console.error('Failed to parse saved slot data:', error);
    }
    
    // Initialize with values from C++
    const loadedSampleArray = data.loadedSample || [];
    const currentDrumType = loadedSampleArray[0];
    console.log(`Can1 ${loadedSampleArray}`);
    
    let sampleName = null;
    if (currentDrumType[0] === 0 && kickSamples[currentDrumType[1]]) {
      // Kick sample
      sampleName = kickSamples[currentDrumType[1]].replace(/_wav$/i, '');
    } else if (currentDrumType[0] === 1 && snareSamples[currentDrumType[1]]) {
      // Snare sample
      sampleName = snareSamples[currentDrumType[1]].replace(/_wav$/i, '');
    }

    return {
      name: sampleName,
      index: currentDrumType[1],
      type: currentDrumType[0]
    };
  });

  // Function to update slot state and persist it
  const updateSlot1 = (newState) => {
    setLoadedSample1(newState);
    try {
      sessionStorage.setItem('slot1', JSON.stringify(newState));
    } catch (error) {
      console.error('Failed to save slot data:', error);
    }
  };

  const sensors = useSensors(
    useSensor(PointerSensor, {
      activationConstraint: { distance: 2 }, // pixels before drag starts
    })
  );

  function handleDragStart(event) {
    setActiveId(event.active.id);
    setActiveLabel(event.active.data?.current?.label || ""); // <— read from data
  }

  function handleDragEnd(event) {
    if (event.over && event.over.id === 'slot1') {
      const dragData = event.active.data?.current;
      const newSlotState = {
        name: dragData.label,
        index: dragData.index,
        type: dragData.type
      };

      updateSlot1(newSlotState);
      loadDrumSample(dragData.type, dragData.index, 0);
    }
    else {
      setActiveId(null);
      setActiveLabel("");
    }
  };

  const dropAnimation = {
    duration: 300,
    sideEffects: defaultDropAnimationSideEffects({
      styles: { active: { opacity: '1' } }, // don’t dim/hide source
    }),
  };



  return (
    <>
      <DndContext autoScroll={false} sensors={sensors} onDragStart={handleDragStart} onDragEnd={handleDragEnd}>
        <div style={{ display: "flex", flexDirection: "column" }}>
          <DrumTypes></DrumTypes>
          <Droppable id="slot1">
            <Slots name={loadedSample1.name} index={loadedSample1.index} type={loadedSample1.type} />
          </Droppable>
          {data.isStandalone[0] && <button style={{
            bottom: "20px", right: "20px",
            fontFamily: "WebFont, sans-serif",
            fontSize: "30px",
            backgroundColor: "black",
            color: "white",
            border: "none",
            position: "absolute"
          }} className="settings" onClick={handleSettings}></button>}
        </div>
        <DragOverlay dropAnimation={dropAnimation} >
          {activeId ? <DrumSample name={activeLabel || activeId} /> : null}
        </DragOverlay>
      </DndContext>
    </>
  )
}

export default App
