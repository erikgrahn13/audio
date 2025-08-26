import { DeathMetalButton, DeathMetalKnob, DeathMetalSlider } from '@abyss-lounge/widgets'

function App() {

  return (
    <>
      {/* <DeathMetalButton parameterName="bypass"></DeathMetalButton> */}
      {/* <DeathMetalKnob parameterName="gain" size={60}></DeathMetalKnob> */}
      <DeathMetalSlider parameterName="gain" width={200}></DeathMetalSlider>
    </>
  )
}

export default App
