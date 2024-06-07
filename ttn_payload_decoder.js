// Decoder function for The Things Network (TTN)
function decodeUplink(input) {
    var data = {};
    var warnings = [];
    var bytes = input.bytes;
  
    try {
      // Convert bytes to string
      var payloadString = String.fromCharCode.apply(null, bytes);
  
      // Split string by comma
      var values = payloadString.split(',');
  
      if (values.length === 2) {
        data.temperature = parseFloat(values[0]);
        data.humidity = parseFloat(values[1]);
      } else {
        warnings.push("Unexpected payload length: " + values.length);
      }
    } catch (error) {
      warnings.push("Error decoding payload: " + error.message);
    }
  
    return {
      data: data,
      warnings: warnings,
    };
  }
  