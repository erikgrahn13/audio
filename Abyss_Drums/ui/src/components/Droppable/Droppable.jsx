import { useDroppable } from "@dnd-kit/core";
import React from "react";

function Droppable({id, data, children}) {
    const { isOver, setNodeRef } = useDroppable({
        id: id
    });

    return React.cloneElement(React.Children.only(children), { isOver, setNodeRef });
}

export default Droppable;