import { DeathMetalButton, DeathMetalKnob } from 'widgets'

function App() {

  return (
    <>
      {/* <DeathMetalButton parameterName="bypass"></DeathMetalButton> */}
      <DeathMetalKnob parameterName="gain" width={60} height={60}></DeathMetalKnob>
    </>
  )
}

export default App
