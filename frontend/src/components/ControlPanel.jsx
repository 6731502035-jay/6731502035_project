import React from 'react';
import './ControlPanel.css';

const ControlPanel = ({ onWater, ipAddress, setIpAddress, loading }) => {
    return (
        <div className="control-panel">
            <div className="input-group">
                <label htmlFor="ip-input">ESP32 IP Address:</label>
                <input
                    id="ip-input"
                    type="text"
                    value={ipAddress}
                    onChange={(e) => setIpAddress(e.target.value)}
                    placeholder="e.g., 192.168.1.100"
                />
            </div>
            <button
                className="water-btn"
                onClick={onWater}
                disabled={loading}
                aria-label="Water the plant"
            >
                {loading ? '🚿 Watering...' : '🚿 Water'}
            </button>
        </div>
    );
};

export default ControlPanel;
