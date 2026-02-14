import { useState, useEffect } from 'react';
import './App.css';
import GravityMeter from './components/GravityMeter';
import ControlPanel from './components/ControlPanel';

function App() {
  const [moisture, setMoisture] = useState(50); // Start at 50% for visual balance
  const [ipAddress, setIpAddress] = useState('192.168.4.1'); // Default ESP32 AP IP
  const [loading, setLoading] = useState(false);

  // Poll moisture level every 2 seconds
  useEffect(() => {
    const interval = setInterval(() => {
      fetch(`http://${ipAddress}/status`)
        .then(res => res.json())
        .then(data => {
          if (data.moisture !== undefined) {
            setMoisture(data.moisture);
          }
        })
        .catch(err => console.log('Polling error:', err));
    }, 2000);

    return () => clearInterval(interval);
  }, [ipAddress]);

  const handleWater = async () => {
    setLoading(true);
    // Optimistic UI update: simulate moisture increasing (floating up)
    // In reality, it takes time for water to seep, but for "gravity" effect we want instant feedback?
    // User asked: "when click Water... instantly turn on Pump... to restore gravity (icon floats back up)"

    // We'll simulate a rise for visual feedback
    const originalMoisture = moisture;
    setMoisture(100); // Float up!

    try {
      await fetch(`http://${ipAddress}/water`, {
        method: 'POST',
      });
      // After pump stops (e.g., 5s), it might naturally go down or stay up.
      // We rely on polling to correct it eventually.
    } catch (error) {
      console.error('Failed to water plants:', error);
      // Revert if failed
      setMoisture(originalMoisture);
      alert('Failed to connect to ESP32. Check IP.');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="dashboard">
      <h1>🌱 Gravity Garden</h1>
      <div className="dashboard-content">
        <GravityMeter moisture={moisture} />
        <ControlPanel
          onWater={handleWater}
          ipAddress={ipAddress}
          setIpAddress={setIpAddress}
          loading={loading}
        />
      </div>
    </div>
  );
}

export default App;
