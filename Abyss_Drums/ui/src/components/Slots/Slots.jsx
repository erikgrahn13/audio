import * as Juce from "juce-framework-frontend";
// import "./Slots.module.css"

const nativeFunction = Juce.getNativeFunction("nativeFunction");


function Slots({isOver, setNodeRef, name, index, type, children}) {

  function handleClick() {
    console.log(`slot clicked ${name} ${index} ${type}`);
    if(!name) return;
    nativeFunction(type, index); // 0 is kicks

  }

  return (
    <button ref={setNodeRef} 
    style={{width: 120,
            height: 120,
            backgroundColor: isOver ? "white" : "gray",
            border: "none",
            borderRadius: "12px",
    }} onClick={handleClick}
        onMouseDown={(e) => { e.currentTarget.style.boxShadow = '0 0 5px yellow'; }}
         onMouseUp={(e) => { e.currentTarget.style.boxShadow = ''; }}
         onMouseLeave={(e) => { e.currentTarget.style.boxShadow = ''; }}
        >
      {name ?? ""} 
    </button>
  );
}

export default Slots;