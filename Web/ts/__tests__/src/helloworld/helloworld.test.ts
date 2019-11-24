import { HelloWorld } from "../../../src/helloworld/helloworld";

describe("sample test", () => {
  test("helloworld", () => {
    const str = "hello";
    const hw = new HelloWorld(str);
    const ret = hw.greet();

    expect(ret).toBe(str);
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
