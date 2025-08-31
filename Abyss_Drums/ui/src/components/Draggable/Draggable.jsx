import React from 'react';
import {useDraggable} from '@dnd-kit/core';
import DrumSample from '../DrumSample/DrumSample';

function Draggable({id, data, children}) {
  const { attributes, listeners, transform, isDragging, setNodeRef } =
    useDraggable({ id, data });

    const style = transform ? {
        transform: isDragging
      ? undefined
      : transform
      ? `translate3d(${transform.x}px, ${transform.y}px, 0)`
      : undefined,
        opacity: isDragging ? 0.6 : 1,
    } : undefined;
  
  return (
        <div ref={setNodeRef} style={style} {...attributes} {...listeners}>
            {children}
        </div>
  );
}

export default Draggable;