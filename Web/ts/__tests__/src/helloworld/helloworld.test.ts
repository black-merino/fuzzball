import { HelloWorld } from "../../../src/helloworld/helloworld";

describe("sample test", () => {
  describe("helloworld", () => {
    const str = "hello";
    const hw = new HelloWorld(str);
    const ret = hw.greet();

    expect(ret).toBe(str);
  });
  afterAll(() => {
    console.log("after all");
  });
});
