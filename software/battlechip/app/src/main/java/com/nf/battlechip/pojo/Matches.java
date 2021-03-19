package com.nf.battlechip.pojo;

import androidx.annotation.Nullable;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Optional;

public class Matches {
    @Nullable
    private List<Match> matches;

    public List<Match> getMatches() {
        return Collections.unmodifiableList(Optional.ofNullable(matches).orElse(new ArrayList<>()));
    }
}
