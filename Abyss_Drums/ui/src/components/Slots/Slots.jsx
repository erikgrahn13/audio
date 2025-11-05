import * as Juce from "juce-framework-frontend";
import React, {useState} from "react";
import { Button } from "@mui/material";
import KickDrum from "../../assets/kick-drum.png";
import SnareDrum from "../../assets/snare10.png";

const playPreviewSample = Juce.getNativeFunction("playPreviewSample");
const clearSample = Juce.getNativeFunction("clearSample");


function Slots({ isOver, setNodeRef, name, index, type, onClear }) {

  const [menuOpen, setMenuOpen] = useState(false);

  function handleClick() {
    if (!name || name === "") return;
    playPreviewSample(type, index);
  }

  function handleRightClick(e) {
    e.preventDefault();
    setMenuOpen(true);
  }

  function handleCloseMenu() {
    setMenuOpen(false);
  }

  // Function to get the appropriate image based on type
  const getImageForType = () => {
    // Safely convert to string and lowercase
    let lowerType = '';
    if (type !== null && type !== undefined) {
      lowerType = String(type).toLowerCase();
    }
    
    switch(lowerType) {
      case 'kick':
      case 'kick_drum':
      case 'kickdrum':
      case '0': // in case type is numeric
        return KickDrum;
      case 'snare':
      case 'snare_drum':
      case 'snaredrum':
      case '1': // in case type is numeric
        return SnareDrum;
      default:
        return KickDrum; // Always return an image
    }
  };

  return (
    <div style={{ display: "flex", justifyContent: "center", alignItems: "center" }}>
      <Button 
        ref={setNodeRef}
        variant="contained"
        sx={{
          padding: "0px",
          margin: "10px",
          backgroundColor: isOver ? "#131313ff" : "black",
          borderRadius: "12px",
          borderColor: "white",
          borderWidth: "1px",
          borderStyle: "solid",
          width: "160px",
          height: "100px",
          minWidth: "120px",
          minHeight: "100px",
          maxWidth: "160px",
          maxHeight: "120px",
          display: "flex",
          flexDirection: "column",
          alignItems: "center",
          transition: 'box-shadow 0.001s ease',
          justifyContent: "center",
          '&:hover': {
            // backgroundColor: isOver ? "#131313ff" : "#555",
          },
          '&:active': {
            boxShadow: '0 0 5px yellow',
          }
        }} 
        onMouseDown={(e) => { 
            if(e.button === 0) {
              handleClick();
            }
        }}
        onContextMenu={handleRightClick}
        disableRipple
      >
        {name && (
          <img 
            src={getImageForType()} 
            alt={type || 'drum'}
            style={{
              width: "80px",
              height: "80px",
              marginBottom: "0px",
              objectFit: "contain",
            }}
          />
        )}
        {name && (
          <span style={{ 
            fontFamily: "DeathMetalFont2",
            fontSize: "12px",
            textAlign: "center",
            wordBreak: "break-word"
          }}>
            {name.replace(/_wav$/i, '')}
          </span>
        )}
      </Button>
{menuOpen && (
  <>
    {/* Overlay to block interaction */}
    <div
      style={{
        position: "fixed",
        top: 0, left: 0, right: 0, bottom: 0,
        background: "rgba(0,0,0,0.3)",
        zIndex: 999
      }}
      onClick={e => e.stopPropagation()} // Prevent closing by clicking overlay
    />
    {/* Modal window */}
    <div
      style={{
        position: "fixed",
        top: "50%",
        left: "50%",
        transform: "translate(-50%, -50%)",
        background: "gray",
        border: "2px solid gray",
        borderRadius: "10px",
        zIndex: 1000,
        padding: "32px",
        boxShadow: "0 4px 24px rgba(0,0,0,0.2)",
        minWidth: "220px",
        textAlign: "center"
      }}
      tabIndex={-1} // Allows focus
      autoFocus // Focuses the modal
    >
      <div style={{ marginBottom: "24px", fontFamily: "DeathMetalFont2", }}>CLEAR SOUND?</div>
      <Button
        variant="contained"
        sx={{ 
          marginRight: "16px", 
          padding: "8px 24px", 
          fontFamily: "DeathMetalFont2",
          backgroundColor: "#d32f2f",
          '&:hover': { backgroundColor: "#b71c1c" }
        }}
        onClick={() => {
          setMenuOpen(false);
          clearSample();
          if(onClear) onClear();
        }}
      >
        YES
      </Button>
      <Button
        variant="outlined"
        sx={{ 
          padding: "8px 24px", 
          fontFamily: "DeathMetalFont2",
          borderColor: "gray",
          color: "black",
          '&:hover': { borderColor: "#555", backgroundColor: "#f5f5f5" }
        }}
        onClick={handleCloseMenu}
      >
        NO
      </Button>
    </div>
  </>
)}
    </div>
  );
}

export default Slots;