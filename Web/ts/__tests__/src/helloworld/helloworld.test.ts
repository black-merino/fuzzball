import { HelloWorld } from "../../../src/helloworld/helloworld";

describe("sample test", () => {
  describe("helloworld", () => {
    const hw = new HelloWorld("hello");
    const str = hw.greet();

    expect(str).toBe("hello");
  });
  afterAll(() => {
    console.log("after all");
  });
});
