import * as Juce from "juce-framework-frontend";
import React, {useState} from "react";

const playPreviewSample = Juce.getNativeFunction("playPreviewSample");
const clearSample = Juce.getNativeFunction("clearSample");


function Slots({ isOver, setNodeRef, name, index, type, onClear }) {

  const [menuOpen, setMenuOpen] = useState(false);
  const [menuPosition, setMenuPosition] = useState({ x: 0, y: 0 });

  function handleClick() {
    if (!name || name === "") return;
    playPreviewSample(type, index);
  }

  function handleRightClick(e) {
    e.preventDefault();
    setMenuOpen(true);
    setMenuPosition({ x: e.clientX, y: e.clientY });
  }

  function handleCloseMenu() {
    setMenuOpen(false);
  }

  return (
    <>
      <button ref={setNodeRef}
        style={{
          padding: 40,
          margin: "10px 80px 0px 80px",
          backgroundColor: isOver ? "white" : "gray",
          fontFamily: "DeathMetalFont2",
          borderRadius: "12px",
        }} onMouseDown={(e) => { 
            if(e.button === 0) {
              e.currentTarget.style.boxShadow = '0 0 5px yellow'; 
              handleClick();
            }
        }}
        onContextMenu={handleRightClick}
        onMouseUp={(e) => { e.currentTarget.style.boxShadow = ''; }}
        onMouseLeave={(e) => { e.currentTarget.style.boxShadow = ''; }}
      >
        {name?.replace(/_wav$/i, '') ?? ""}
      </button>
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
      <button
        style={{ marginRight: "16px", padding: "8px 24px", fontFamily: "DeathMetalFont2", }}
        onClick={() => {
          setMenuOpen(false);
          clearSample();
          if(onClear) onClear();
        }}
      >
        YES
      </button>
      <button
        style={{ padding: "8px 24px", fontFamily: "DeathMetalFont2", }}
        onClick={handleCloseMenu}
      >
        NO
      </button>
    </div>
  </>
)}
    </>
    
  );
}

export default Slots;