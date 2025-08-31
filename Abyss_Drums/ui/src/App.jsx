import './App.css'
import DrumTypes from "./components/DrymType/DrumType";
import ListItemButton from '@mui/material/ListItemButton';
import ListItemText from '@mui/material/ListItemText';
import DrumSample from './components/DrumSample/DrumSample';
import Slots from "./components/Slots/Slots"
import Droppable from './components/Droppable/Droppable';
import React, { useState } from 'react';
import { DeathMetalSlider } from "@abyss-lounge/components";
import * as Juce from "juce-framework-frontend";



import { DndContext, PointerSensor, useSensor, useSensors, DragOverlay, defaultDropAnimationSideEffects } from "@dnd-kit/core"



function App() {
  const data = window.__JUCE__.initialisationData;
  const openSettingsView = Juce.getNativeFunction("openSettingsView");
  
  const handleSettings = (e) => {
    openSettingsView("one", 2, null).then((result) => { });
  }

  const [isDropped, setIsDropped] = useState(false);
  const [activeId, setActiveId] = useState(null);
  const [activeLabel, setActiveLabel] = useState("");
  const [loadedSample1, setLoadedSample1] = useState({
    name: null,
    index: 0,
    type: null
  });
  const [loadedSample2, setLoadedSample2] = useState({
    name: null,
    index: 0,
    type: null
  });
  const [loadedSample3, setLoadedSample3] = useState({
    name: null,
    index: 0,
    type: null
  });
  const [loadedSample4, setLoadedSample4] = useState({
    name: null,
    index: 0,
    type: null
  });
  const [loadedSample5, setLoadedSample5] = useState({
    name: null,
    index: 0,
    type: null
  });


  const sensors = useSensors(
    useSensor(PointerSensor, {
      activationConstraint: { distance: 0 }, // pixels before drag starts
    })
  );

  function handleDragStart(event) {
    setActiveId(event.active.id);
    setActiveLabel(event.active.data?.current?.label || ""); // <— read from data
  }

  function handleDragEnd(event) {
    console.log(`erik4 ${event.over.id}`);
    if (event.over && event.over.id === 'slot1') {
      console.log(`erik ${event.active.data?.current?.index}`);
      const data = event.active.data?.current;
      setLoadedSample1({
        name: data.label,
        index: data.index,
        type: data.type

      });
    }
    else if (event.over && event.over.id === 'slot2') {
      console.log(`erik ${event.active.data?.current?.index}`);
      const data = event.active.data?.current;
      setLoadedSample2({
        name: data.label,
        index: data.index,
        type: data.type

      });
    }
    else if (event.over && event.over.id === 'slot3') {
      console.log(`erik ${event.active.data?.current?.index}`);
      const data = event.active.data?.current;
      setLoadedSample3({
        name: data.label,
        index: data.index,
        type: data.type

      });
    }
    else if (event.over && event.over.id === 'slot4') {
      console.log(`erik ${event.active.data?.current?.index}`);
      const data = event.active.data?.current;
      setLoadedSample4({
        name: data.label,
        index: data.index,
        type: data.type

      });
    }
    else if (event.over && event.over.id === 'slot5') {
      const data = event.active.data?.current;
      console.log(`erik5 ${data.name} ${data.index} ${data.type}`);
      setLoadedSample5({
        name: data.label,
        index: data.index,
        type: data.type

      });
    }

    setActiveId(null);
    setActiveLabel("");
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
        <div style={{ display: "flex", flexDirection: "column", minHeight: '100vh' }}>
          <DrumTypes></DrumTypes>
          <div style={{
            position: 'relative',
            flex: '1 1 auto',
            display: 'flex',
            // flexDirection: 'column',
            border: '1px solid white',
            borderRadius: 10,
            marginBottom: 20,
            gap: 20,
            padding: '16px 16px 96px 16px', // bottom padding ~= bar height
          }}>
            <div style={{ border: "1px solid white", borderRadius: 12 }}>
              <button>M</button>
              <button>S</button>
              <DeathMetalSlider orientation='vertical' width={24} height={200}></DeathMetalSlider>
              <br />
              <Droppable id="slot1">
                <Slots name={loadedSample1.name} index={loadedSample1.index} type={loadedSample1.type} />
              </Droppable>
            </div>
            <div style={{ border: "1px solid white", borderRadius: 12 }}>
              <DeathMetalSlider orientation='vertical' width={24} height={200}></DeathMetalSlider>
              <br />
              <Droppable id="slot2">
                <Slots name={loadedSample2.name} index={loadedSample2.index} type={loadedSample2.type} />
              </Droppable>
            </div>
            <div style={{ border: "1px solid white", borderRadius: 12 }}>
              <DeathMetalSlider orientation='vertical' width={24} height={200}></DeathMetalSlider>
              <br />
              <Droppable id="slot3">
                <Slots name={loadedSample3.name} index={loadedSample3.index} type={loadedSample3.type} />
              </Droppable>
            </div>
            <div style={{ border: "1px solid white", borderRadius: 12 }}>
              <DeathMetalSlider orientation='vertical' width={24} height={200}></DeathMetalSlider>
              <br />
              <Droppable id="slot4">
                <Slots name={loadedSample4.name} index={loadedSample4.index} type={loadedSample4.type} />
              </Droppable>
            </div>
            <div style={{ border: "1px solid white", borderRadius: 12 }}>
              <DeathMetalSlider orientation='vertical' width={24} height={200}></DeathMetalSlider>
              <br />
              <Droppable id="slot5">
                <Slots name={loadedSample5.name} index={loadedSample5.index} type={loadedSample5.type} />
              </Droppable>
            </div>

          </div>
        </div>

        {/* </div> */}
        <DragOverlay dropAnimation={dropAnimation} >
          {activeId ? <DrumSample name={activeLabel || activeId} /> : null}
        </DragOverlay>
      </DndContext>
      {data.isStandalone[0] && <button className="settings" onClick={handleSettings}></button>}
    </>
  )
}

export default App
