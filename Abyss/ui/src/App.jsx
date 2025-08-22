import { DeathMetalButton, DeathMetalKnob } from 'widgets'

function App() {

  return (
    <>
      {/* <DeathMetalButton parameterName="bypass"></DeathMetalButton> */}
      <DeathMetalKnob parameterName="gain" size={200}></DeathMetalKnob>
    </>
  )
}

export default App
