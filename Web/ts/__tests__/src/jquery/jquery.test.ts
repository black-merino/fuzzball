//
// Try to use HTML document and jquery in Typescript
//
import * as $ from 'jquery';
import { JQWrapper } from "../../../src/jquery/jqwrapper";

describe("sample test", () => {
  test("jsdom", () => {
    const elem = document.createElement("div");
    elem.innerHTML =
      "<div id='c1'><div id='id1'><div/><div/><div id='c2'/>";

    console.log(elem.outerHTML);
  });

  test("jquery", () => {
    const elem = document.createElement("div");
    elem.innerHTML =
      "<div id='c1'><div id='id1'><div/><div/><div id='c2'/>";

    const $elem = $(elem);

    $elem.attr('disabled','');
    $elem.removeAttr('disabled');
  });

  test("JQWrapper", () => {
    (<any>(top)).$ = $;

    const elem = document.createElement("div");
    elem.innerHTML =
      "<div id='c1'><div id='id1'><div/><div/><div id='c2'/>";

    const jqw = new JQWrapper(elem);
    jqw.enable();
    jqw.disable();

  });
});
