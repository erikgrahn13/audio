import React from "react";
import ListItemButton from '@mui/material/ListItemButton';
import ListItemText from '@mui/material/ListItemText';
import "./DrumSample.module.css"

function DrumSample({ id, name, onClick, selected = false }) {

  return (
    <ListItemButton

      onMouseDown={onClick}
      selected={selected}
      sx={{
        '& .MuiListItemText-primary': {
          fontFamily: '"DeathMetalFont2", sans-serif'
        },
        "&.Mui-selected": { backgroundColor: "gray" },        // gray-800
        "&.Mui-selected:hover": { backgroundColor: "gray" },  // gray-900
      }}
    >
      <ListItemText primary={name} />
    </ListItemButton>
  );
}

export default DrumSample;