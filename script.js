Promise.all([
    fetch('tree.json').then(res => res.json()),
    fetch('trace.json').then(res => res.json())
]).then(([treeData, traceData]) => {
    renderTree(treeData, traceData);
});

// Dark theme color palette
function getNodeColor(label) {
    if (label.startsWith("Function")) return "url(#func-gradient)";
    if (label.startsWith("If")) return "#ffb4a2";
    if (label.startsWith("While")) return "#b5838d";
    if (label.startsWith("Assignment")) return "#80ffdb";
    if (label.startsWith("Return")) return "#ffd166";
    if (label.startsWith("VarDecl")) return "#5e60ce";
    if (label.startsWith("FunctionCall")) return "#ff6f61";
    if (label.startsWith("Parameters")) return "#56cfe1";
    if (label.startsWith("Arguments")) return "#72efdd";
    if (label.startsWith("Expr")) return "#5390d9";
    if (label.startsWith("Block") || label.startsWith("Body")) return "#3a0ca3";
    if (label.startsWith("Program")) return "#f8961e";
    return "#495057";
}

function renderTree(treeData, traceData) {
    const margin = { top: 80, right: 120, bottom: 10, left: 120 };
    const width = 1800 - margin.left - margin.right;
    const height = 1400 - margin.top - margin.bottom;

    d3.select("body")
        .style("background", "#181825")
        .style("color", "#f1faee");

    d3.select("#tree").selectAll("svg").remove();

    const svg = d3.select("#tree")
        .append("svg")
        .attr("width", "100%")
        .attr("height", "100%")
        .attr("viewBox", `0 0 ${width + margin.left + margin.right} ${height + margin.top + margin.bottom}`)
        .attr("preserveAspectRatio", "xMidYMid meet")
        .style("background", "#181825");

    svg.append("defs").append("linearGradient")
        .attr("id", "func-gradient")
        .attr("x1", "0%").attr("y1", "0%")
        .attr("x2", "100%").attr("y2", "100%")
        .selectAll("stop")
        .data([
            { offset: "0%", color: "#3a86ff" },
            { offset: "100%", color: "#4361ee" }
        ])
        .enter()
        .append("stop")
        .attr("offset", d => d.offset)
        .attr("stop-color", d => d.color);

    const g = svg.append("g")
        .attr("transform", `translate(${margin.left},${margin.top})`);

    svg.call(
        d3.zoom()
            .scaleExtent([0.1, 3])
            .on("zoom", (event) => {
                g.attr("transform", event.transform);
            })
    );

    const root = d3.hierarchy(treeData);

    // Tree layout size switched for vertical tree
    const treeLayout = d3.tree()
        .size([width, height])  // width horizontal, height vertical
        .separation((a, b) => (a.parent === b.parent ? 2.7 : 2));

    treeLayout(root);

    // Use vertical link generator
    g.selectAll('.link')
        .data(root.links())
        .enter()
        .append('path')
        .attr('class', 'link')
        .attr('fill', 'none')
        .attr('stroke', '#353945')
        .attr('stroke-width', 1.5)
        .attr('stroke-opacity', 0.85)
        .attr('d', d3.linkVertical()
            .x(d => d.x)
            .y(d => d.y));

    const node = g.selectAll('.node')
        .data(root.descendants())
        .enter()
        .append('g')
        .attr('class', 'node')
        .attr('transform', d => `translate(${d.x},${d.y})`);

    node.append('circle')
        .attr('r', 14)
        .attr('fill', d => getNodeColor(d.data.name))
        .attr('stroke', '#f1faee')
        .attr('stroke-width', 2)
        .style('filter', 'drop-shadow(0 2px 8px rgba(0,0,0,0.25))')
        .on('mouseover', function () {
            d3.select(this)
                .attr('stroke', '#ffd166')
                .attr('stroke-width', 3);
        })
        .on('mouseout', function () {
            d3.select(this)
                .attr('stroke', '#f1faee')
                .attr('stroke-width', 2);
        });

    node.append('text')
        .attr('dy', 5)
        .attr('y', d => d.children ? -20 : 20) // place text above parent nodes, below leaves
        .attr('text-anchor', 'middle')
        .style('font-size', '13px')
        .style('font-family', '"Segoe UI", Roboto, sans-serif')
        .style('fill', '#f1faee')
        .text(d => d.data.name);

    // No animation/highlighting for now
}
