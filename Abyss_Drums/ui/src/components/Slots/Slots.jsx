import * as Juce from "juce-framework-frontend";

const playPreviewSample = Juce.getNativeFunction("playPreviewSample");


function Slots({ isOver, setNodeRef, name, index, type }) {

  function handleClick() {
    if (!name) return;
    playPreviewSample(type, index);
  }

  return (
    <button ref={setNodeRef}
      style={{
        padding: 40,
        margin: "10px 80px 0px 80px",
        backgroundColor: isOver ? "white" : "gray",
        fontFamily: "DeathMetalFont2",
        borderRadius: "12px",
      }} onMouseDown={(e) => { 
        e.currentTarget.style.boxShadow = '0 0 5px yellow'; 
        handleClick();
      }}
      onMouseUp={(e) => { e.currentTarget.style.boxShadow = ''; }}
      onMouseLeave={(e) => { e.currentTarget.style.boxShadow = ''; }}
    >
      {name?.replace(/_wav$/i, '') ?? ""}
    </button>
  );
}

export default Slots;