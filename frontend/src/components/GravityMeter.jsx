import React from 'react';
import './GravityMeter.css';

const GravityMeter = ({ moisture }) => {
  // moisture is 0-100
  // 100% = Floating (Top)
  // 0% = Fallen (Bottom)
  
  // Calculate position: 0% moisture -> bottom: 0%, 100% moisture -> bottom: 80% (keep inside container)
  const bottomPosition = (moisture / 100) * 80 + 5; 

  return (
    <div className="gravity-meter-container">
      <div className="meter-label">Gravity Meter</div>
      <div className="meter-track">
        <div 
          className="floater" 
          style={{ bottom: `${bottomPosition}%` }}
        >
          <span role="img" aria-label="water-drop" className="icon">💧</span>
        </div>
        <div className="ground-level"></div>
      </div>
      <div className="moisture-value">{moisture}% Moisture</div>
    </div>
  );
};

export default GravityMeter;
