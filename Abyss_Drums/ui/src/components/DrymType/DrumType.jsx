
import * as Juce from "juce-framework-frontend";
import "./DrumType.module.css"
import React, { useState, useRef } from "react";
import { createPortal } from "react-dom";
import Box from '@mui/material/Box';
import Paper from '@mui/material/Paper';
import List from '@mui/material/List';
import ListItemButton from '@mui/material/ListItemButton';
import ListItemText from '@mui/material/ListItemText';
import ListSubheader from '@mui/material/ListSubheader';
import DrumSample from "../DrumSample/DrumSample";
import Draggable from "../Draggable/Draggable";
import { useDndMonitor } from "@dnd-kit/core";
import {
  DndContext,
  DragOverlay,
  PointerSensor,
  useSensor,
  useSensors,
} from "@dnd-kit/core";

const kickSamples = window.__JUCE__?.initialisationData?.kickSamples[0];
const snareSamples = window.__JUCE__?.initialisationData?.snareSamples[0];
const nativeFunction = Juce.getNativeFunction("nativeFunction");


export default function DrumColumns() {
  const paperRef = useRef(null);
  const lockedRef = useRef(false);
  const [selectedId, setSelectedId] = useState(null);

  useDndMonitor({
    onDragStart() {
      lockedRef.current = true;
      if (paperRef.current) paperRef.current.style.overflowY = "hidden";

    },
    onDragEnd() {
      lockedRef.current = false;
      if (paperRef.current) paperRef.current.style.overflowY = "auto";

    },
    onDragCancel() {
      lockedRef.current = false;
      if (paperRef.current) paperRef.current.style.overflowY = "auto";
    },
  });

  function handleKick(event, itemId) {
    nativeFunction(0, itemId); // 0 is kicks
  }

  function handleSnare(event, itemId) {
    nativeFunction(1, itemId); // 1 is snares
  }

  return (
    <Box sx={{ height: '50vh', display: 'grid', gridTemplateColumns: { xs: '1fr', md: '1fr 1fr 1fr' }, gap: 1, p: 1 }}>
      <Paper ref={paperRef} sx={{ bgcolor: "black", color: "white", overflowY: "auto", border: "1px white solid" }}>
        <List dense subheader={<ListSubheader sx={{ bgcolor: "black", color: "white" }}>Kicks</ListSubheader>}>
          {kickSamples.map((name, i) => {
            const id = `kick-${i}`;
            return (
              <Draggable key={id} id={id} data={{ label: name, index: i, type: 0 }}> 
                <DrumSample
                  name={name}
                  selected={selectedId === id}
                  onClick={() => {
                    setSelectedId(id);        // highlight this row
                    handleKick(name, i);      // your existing click action
                  }}
                />
              </Draggable>
            );
          })}
        </List>
      </Paper>

      <Paper ref={paperRef} sx={{ bgcolor: "black", color: "white", overflowY: "auto", border: "1px white solid" }}>
        <List dense subheader={<ListSubheader sx={{ bgcolor: "black", color: "white" }}>Snares</ListSubheader>}>
          {snareSamples.map((name, i) => {
            const id = `snare-${i}`;
            return (
              <Draggable key={id} id={id} data={{ label: name, index: i, type: 1 }}> 
                <DrumSample
                  name={name}
                  selected={selectedId === id}
                  onClick={() => {
                    setSelectedId(id);        // highlight this row
                    handleSnare(name, i);      // your existing click action
                  }}
                />
              </Draggable>
            );
          })}
        </List>
      </Paper>

      <Paper sx={{ bgcolor: "black", color: "white", overflowY: 'auto', border: "1px white solid" }}>
        <List dense subheader={<ListSubheader sx={{ bgcolor: "black", color: "white" }}>Snares</ListSubheader>}>
          {snareSamples.map((name, i) => (
            <ListItemButton key={`snare-${i}`} onClick={() => handleSnare(name, i)}>
              <ListItemText primary={name} />
            </ListItemButton>
          ))}
        </List>
      </Paper>
    </Box>

  );
}