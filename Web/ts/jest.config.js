module.exports = {
  preset: "ts-jest",
  //  testEnvironment: "node",
  testEnvironment: "jest-environment-jsdom-global",
  globals: {
    $: {},
    document: {}
  }
};
