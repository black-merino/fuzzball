import * as $ from 'jquery';

export class JQWrapper {
  private $elem : any;

  constructor(elem : any) {
    const jq = (<any>top).$;
    this.$elem = jq(elem);
  }

  public enable(){
    this.$elem.removeAttr('disabled');
  }
  public disable(){
    this.$elem.attr('disabled','');
  }
}

