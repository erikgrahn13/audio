import { DeathMetalSwitch, DeathMetalKnob, DeathMetalSlider } from '@abyss-lounge/components'

function App() {

  return (
    <>
      {/* <DeathMetalSwitch parameterName="bypass"></DeathMetalSwitch> */}
      {/* <DeathMetalKnob parameterName="gain" size={60}></DeathMetalKnob> */}
      <DeathMetalSlider parameterName="gain" width={200}></DeathMetalSlider>
    </>
  )
}

export default App
