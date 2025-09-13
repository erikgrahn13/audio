
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
const playPreviewSample = Juce.getNativeFunction("playPreviewSample");


export default function DrumColumns() {
  const paperRef = useRef(null);
  const lockedRef = useRef(false);
  const [selectedId, setSelectedId] = useState(null);

  function handleKick(event, itemId) {
    playPreviewSample(0, itemId); // 0 is kicks
  }

  function handleSnare(event, itemId) {
    playPreviewSample(1, itemId); // 1 is snares
  }

  return (
    <Box sx={{ height: '52vh', minHeight: "410px", display: 'grid', gridTemplateColumns: 'repeat(2, 1fr)', gap: 1, p: 1 }}>
      <Paper ref={paperRef} sx={{
        bgcolor: "black", color: "white", overflowY: "auto", border: "1px white solid", "&::-webkit-scrollbar": { width: 8 },
        "&::-webkit-scrollbar-track": { background: "black" },
        "&::-webkit-scrollbar-thumb": {
          backgroundColor: "gray",
          borderRadius: 8,
          // border: "1px solid white",
        },
      }}>
        <List dense subheader={<ListSubheader sx={{ bgcolor: "black", color: "white", fontFamily: "DeathMetalFont2" }}>Kicks</ListSubheader>} sx={{ fontFamily: "DeathMetalFont2" }}>
          {kickSamples.map((name, i) => {
            const id = `kick-${i}`;
            return (
              <Draggable key={id} id={id} data={{ label: name, index: i, type: 0 }}>
                <DrumSample
                  name={name.replace(/_wav$/i, '')}
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

      <Paper ref={paperRef} sx={{
        bgcolor: "black", color: "white", overflowY: "auto", border: "1px white solid", "&::-webkit-scrollbar": { width: 8 },
        "&::-webkit-scrollbar-track": { background: "black" },
        "&::-webkit-scrollbar-thumb": {
          backgroundColor: "gray",
          borderRadius: 8,
          // border: "2px solid black",
        },
      }}>
        <List dense subheader={<ListSubheader sx={{ bgcolor: "black", color: "white", fontFamily: "DeathMetalFont2" }}>Snares</ListSubheader>} sx={{ fontFamily: "DeathMetalFont2" }}>
          {snareSamples.map((name, i) => {
            const id = `snare-${i}`;
            return (
              <Draggable key={id} id={id} data={{ label: name, index: i, type: 1 }}>
                <DrumSample
                  name={name.replace(/_wav$/i, '')}
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
    </Box>

  );
}