(function () {
  function renderMermaid() {
    if (typeof mermaid === "undefined") {
      return;
    }

    mermaid.initialize({
      startOnLoad: false,
      securityLevel: "loose",
      theme: "neutral"
    });

    mermaid.run({ querySelector: ".mermaid" });
  }

  if (typeof document$ !== "undefined") {
    document$.subscribe(renderMermaid);
  } else {
    window.addEventListener("load", renderMermaid);
  }
})();
