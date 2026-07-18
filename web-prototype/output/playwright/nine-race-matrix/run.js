async (page) => {
  const raceIds = await page.evaluate(() => window.__GRAPHYSX_DEBUG__.raceIds());
  const results = [];

  await page.setViewportSize({ width: 1440, height: 900 });

  for (let index = 0; index < raceIds.length; index += 1) {
    const raceId = raceIds[index];
    const immediate = await page.evaluate((id) => {
      window.__GRAPHYSX_DEBUG__.selectRace(id);
      return window.__GRAPHYSX_DEBUG__.snapshot();
    }, raceId);

    await page.waitForFunction(
      () => {
        const loadState = window.__GRAPHYSX_DEBUG__.snapshot().loadState;
        return loadState.ready || Boolean(loadState.error);
      },
      undefined,
      { timeout: 30000 }
    );

    const ready = await page.evaluate(() => window.__GRAPHYSX_DEBUG__.snapshot());
    if (ready.loadState.error) {
      throw new Error(`${raceId} load failed: ${ready.loadState.error}`);
    }

    const started = await page.evaluate(() => window.__GRAPHYSX_DEBUG__.startRace());
    if (!started) {
      throw new Error(`${raceId} refused to start after assets reported ready`);
    }

    await page.waitForTimeout(300);
    const gameplayStart = await page.evaluate(() => window.__GRAPHYSX_DEBUG__.snapshot());
    await page.keyboard.down("ArrowUp");
    await page.waitForTimeout(450);
    await page.keyboard.up("ArrowUp");
    await page.waitForTimeout(100);
    const gameplay = await page.evaluate(() => window.__GRAPHYSX_DEBUG__.snapshot());
    await page.screenshot({
      path: `output/playwright/nine-race-matrix/gameplay-${String(index + 1).padStart(2, "0")}-${raceId}.png`,
      type: "png"
    });

    await page.evaluate(() => window.__GRAPHYSX_DEBUG__.completeObjective());
    await page.waitForFunction(() => window.__GRAPHYSX_DEBUG__.state() === "after-race", undefined, { timeout: 5000 });
    const finished = await page.evaluate(() => ({
      state: window.__GRAPHYSX_DEBUG__.state(),
      snapshot: window.__GRAPHYSX_DEBUG__.snapshot(),
      panelText: document.querySelector(".panel")?.textContent ?? ""
    }));
    await page.screenshot({
      path: `output/playwright/nine-race-matrix/finish-${String(index + 1).padStart(2, "0")}-${raceId}.png`,
      type: "png"
    });

    results.push({
      raceId,
      raceName: gameplay.raceName,
      immediateLoadState: immediate.loadState,
      readyLoadState: ready.loadState,
      ringsAtStart: gameplay.ringsTotal,
      zombiesAtStart: gameplay.zombiesRemaining,
      movementDistance: Number(
        Math.hypot(
          gameplay.playerPosition.x - gameplayStart.playerPosition.x,
          gameplay.playerPosition.z - gameplayStart.playerPosition.z
        ).toFixed(3)
      ),
      finishState: finished.state,
      raceFinished: finished.snapshot.raceFinished,
      ringsAtFinish: `${finished.snapshot.ringsCollected}/${finished.snapshot.ringsTotal}`,
      zombiesAtFinish: finished.snapshot.zombiesRemaining,
      panelHasRaceComplete: finished.panelText.includes("Race Complete")
    });
  }

  return results;
}
