import * as Juce from "./juce/index.js";

const DrumType = {
    Kicks: 0,
    Snare: 1,
    Toms: 2
};


const kickSamples = window.__JUCE__?.initialisationData?.kickSamples[0];
const snareSamples = window.__JUCE__?.initialisationData?.snareSamples[0];

// console.log("sampleList typeof:", typeof kickSamples);     // should be object
// console.log("kickSamples isArray:", Array.isArray(kickSamples)); // should be true
// console.log("kickSamples raw:", kickSamples); 


let kickList = document.querySelector(".kick-samples");
let snareList = document.querySelector(".snare-samples");
const nativeFunction = Juce.getNativeFunction("nativeFunction");


document.querySelectorAll('#categoryColumn .item').forEach(item => {
    item.addEventListener('click', () => {
      const category = item.dataset.category;

            // Reset all backgrounds
    document.querySelectorAll('#categoryColumn .item').forEach(i => {
        i.style.backgroundColor = '';
      });
  
      // Highlight selected
      item.style.backgroundColor = 'grey';

      selectCategory(category);
    });
  });
  
  function selectCategory(category) {
    const browser = document.querySelector('.browser');
    console.log("erik " + category);
  
    // Remove all columns after the first
    while (browser.children.length > 1) {
      browser.removeChild(browser.lastChild);
    }
    console.log("erik2");
  
    // Create new column with sample items
    const column = document.createElement('div');
    column.className = 'column';

    let samples = [];
    let type;

    if(category === "Kicks")
    {

        samples = kickSamples;
        console.log("erik3");
        type = DrumType.Kicks;
    }
    else if(category === "Snares")
    {
        samples = snareSamples;
        type = DrumType.Snare;

    }
  

    samples.forEach((sample, i) => {
      console.log(`Sample ${i}: ${sample}`);
      const item = document.createElement('div');
      item.className = 'sample';
      item.textContent = sample;
      column.appendChild(item); // Make sure `column` is defined!

      item.addEventListener("mousedown", (e) => {
        const items = Array.from(column.children); // all <li> elements
        const index = items.indexOf(e.target);
        console.log(index);

                    // Reset all backgrounds
    document.querySelectorAll('.sample').forEach(i => {
        i.style.backgroundColor = '';
      });
  
      // Highlight selected
      item.style.backgroundColor = 'grey';
        
        nativeFunction(type, index).then((result) => {});
      })
      
    });
  
    browser.appendChild(column);
  }



// kickSamples.forEach((name, index) => {
//     console.log(`Sample ${index}:`, name);
//     let element = document.createElement("li");
//     element.textContent = name;
//     kickList.appendChild(element);

//     element.addEventListener("mousedown", (e) => {
//         console.log(e.target);
//         const items = Array.from(kickList.children); // all <li> elements
//         const index = items.indexOf(e.target);
//         console.log(index);
//     nativeFunction(DrumType.Kicks, index).then((result) => {});

// });
// });

// snareSamples.forEach((name, index) => {
//     console.log(`Sample ${index}:`, name);
//     let element = document.createElement("li");
//     element.textContent = name;
//     snareList.appendChild(element);

//     element.addEventListener("mousedown", (e) => {
//         console.log(e.target);
//         const items = Array.from(snareList.children); // all <li> elements
//         const index = items.indexOf(e.target);
//         console.log(index);
//     nativeFunction(DrumType.Snare, index).then((result) => {});

// });
// });
