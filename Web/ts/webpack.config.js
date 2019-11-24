const path = require("path");

module.exports = {
  mode: "development",
  entry: "./_build/src/circularreference/index.js",
  output: {
    filename: "webpack.js",
    path: path.join(__dirname, "public/circularreference/js")
  }
};
