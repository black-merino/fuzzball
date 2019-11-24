import { HelloWorld } from "../../../src/helloworld/helloworld";
import { JSDOM } from "jsdom";

describe("sample test", () => {
  test("helloworld", () => {
    const str = "hello";
    const hw = new HelloWorld(str);
    const ret = hw.greet();

    expect(ret).toBe(str);
  });
  test("jsdom", () => {
    JSDOM.fromURL(`https://example.com/`).then(dom => {
      //      console.log(dom.serialize());
    });

    const elem = document.createElement("div");
    elem.innerHTML =
      "<div id='id1'><div id='id2'><div id='id3'><div id='id4'><div/><div/><div/><div/><div id='c2'/>";
  });
  beforeAll(() => {
    // console.log("before all");
  });
  beforeEach(() => {
    // console.log("before each");
  });
  afterEach(() => {
    // console.log("after each");
  });
  afterAll(() => {
    // console.log("after all");
  });
});
