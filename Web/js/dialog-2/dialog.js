/**
 * Create Tilte
 */
function myCreateTitle(title) {
  // create Title
  var headerTitle = document.createElement("p");
  headerTitle.className = "dialog-title";
  headerTitle.innerText = title;

  // add to header
  var header = document.createElement("div");
  header.className = "dialog-header";
  header.appendChild(headerTitle);

  return header;
}

/**
 * Create Body
 */
function myCreateBody(iconImage, messages) {
  var body = document.createElement("div");
  var table = document.createElement("table");
  table.className = "dialog-body-table";

  var tr1;
  messages.forEach(function(value) {
    tr1 = document.createElement("tr");
    td = document.createElement("td");
    td.innerText = value;
    tr1.appendChild(td);
    table.appendChild(tr1);
  });

  // create icon element
  var imageTd = document.createElement("td");
  imageTd.rowSpan = 5;
  var icon = document.createElement("img");
  icon.className = "dialog-icon-image";
  icon.src = iconImage;
  imageTd.appendChild(icon);

  // insert icon element to head of first row
  var firstTr = table.children[0];
  firstTr.insertBefore(imageTd, firstTr.childNodes[0]);

  body.appendChild(table);

  return body;
}

// Create button for footer
function myCreateButtonElement(dialog, buttonType) {
  var returnElement = document.createElement("button");
  returnElement.id = "dialog-" + buttonType;
  returnElement.type = "button";
  returnElement.innerText = buttonType;
  returnElement.className = ".dialog-button";
  returnElement.addEventListener("click", function() {
    dialog.close(buttonType);
  });

  return returnElement;
}

/*
 * Create Footer
 */
function myCreateFooter(dialog, buttons) {
  var footer = document.createElement("footer");
  footer.className = "dialog-footer";

  buttons.forEach(function(value) {
    var buttonElement = myCreateButtonElement(dialog, value);
    footer.appendChild(buttonElement);
  });

  return footer;
}

/**
 * Create dialog.
 */
async function myCreateDialog(title, iconImage, messages, buttons) {
  // create Dialog Element.
  var dialog = document.createElement("dialog");

  // create header(title)
  var header = myCreateTitle(title);
  // add header to dialog
  dialog.appendChild(header);

  // create body
  var body = myCreateBody(iconImage, messages);
  // add body to dialog
  dialog.appendChild(body);

  // create footer
  var footer = myCreateFooter(dialog, buttons);
  // add footer to dialog
  dialog.appendChild(footer);

  document.body.appendChild(dialog);

  function addCloseDialogEvent() {
    const p = new Promise((resolve, reject) => {
      dialog.addEventListener("close", function() {
        resolve(dialog.returnValue);
      });
    });

    return p;
  }

  // add close dialog event
  const promise = addCloseDialogEvent();

  dialog.showModal();

  // wait for close dialog event
  await promise;

  return dialog.returnValue;
}
