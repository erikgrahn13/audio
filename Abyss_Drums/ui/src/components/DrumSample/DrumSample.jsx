import React from "react";
import ListItemButton from '@mui/material/ListItemButton';
import ListItemText from '@mui/material/ListItemText';
import { useDraggable } from "@dnd-kit/core";

function DrumSample({id, name, onClick, selected = false}) {

  return (
    <ListItemButton
      onClick={onClick}
      selected={selected}
      sx={{
        "&.Mui-selected": { backgroundColor: "#1f2937" },        // gray-800
        "&.Mui-selected:hover": { backgroundColor: "#111827" },  // gray-900
      }}
    >
      <ListItemText primary={name} />
    </ListItemButton>
  );
}

export default DrumSample;