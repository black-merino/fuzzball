/**
 * Get ip address.
 */
async function getIpAddress() {
  var myIP;

  window.RTCPeerConnection =
    window.RTCPeerConnection ||
    window.mozRTCPeerConnection ||
    window.webkitRTCPeerConnection;
  // create RTCPeerConnection. (ice server not spceify)
  var pc = new RTCPeerConnection({ iceServers: [] });
  var noop = function() {};
  // create data channel
  pc.createDataChannel("");

  function waitCandidate() {
    const p = new Promise((resolve, reject) => {
      pc.onicecandidate = function(ice) {
        if (ice && ice.candidate && ice.candidate.candidate) {
          console.log(ice.candidate.candidate);
          // extract ip address with regular expression
          // myIP = /([0-9]{1,3}(\.[0-9]{1,3}){3}|[a-f0-9]{1,4}(:[a-f0-9]{1,4}){7})/.exec(
          //   ice.candidate.candidate
          // )[1];
          pc.onicecandidate = noop;
          myIP = ice.candidate.candidate;
          resolve(myIP);
        }
      };
    });
    return p;
  }

  var promise = waitCandidate();

  // send offer
  pc.createOffer(pc.setLocalDescription.bind(pc), noop);

  await promise;

  return myIP;
}
