/* Post-experiment audit only. The original acceptance client stays unchanged. */
static void audit_operational(IEcoUnknown* held, int lab2) {
    IEcoLab65* sorter = 0;
    IEcoLab2* version = 0;
    int32_t data[3] = {9, -7, 2};
    arithmetic(held);
    check(held->pVTbl->QueryInterface(held, &IID_IEcoLab65,
        (void**)&sorter) == 0 && sorter, "audit_sorter_after_initial_release");
    if (sorter) {
        check(sorter->pVTbl->SortInt(sorter, data, 3, 7) == 0 &&
            data[0] == -7 && data[1] == 2 && data[2] == 9,
            "audit_sort_after_initial_release");
        sorter->pVTbl->Release(sorter);
    }
    if (lab2) {
        check(held->pVTbl->QueryInterface(held, &IID_IEcoLab2,
            (void**)&version) == 0 && version, "audit_version_available");
        if (version) {
            check(version->pVTbl->GetVersion(version) == 2,
                "audit_version_after_initial_release");
            version->pVTbl->Release(version);
        }
    }
}

static void audit_hold_each_public(int lab2) {
    const UGUID* ids[5];
    IEcoComponentFactory* factory = lab2 ? lab2_factory : lab1_factory;
    const UGUID* initial = lab2 ? &IID_IEcoLab2 : &IID_IEcoLab65;
    IEcoUnknown *component = 0, *held = 0, *before = 0, *after = 0;
    unsigned int i;
    ids[0] = &IID_IEcoUnknown;
    ids[1] = &IID_IEcoLab65;
    ids[2] = &IID_IEcoCalculatorX;
    ids[3] = &IID_IEcoCalculatorY;
    ids[4] = &IID_IEcoLab2;
    for (i = 0; i < (lab2 ? 5U : 4U); i++) {
        reset_mock();
        component = 0;
        held = 0;
        if (!check(factory->pVTbl->Alloc(factory, &mock.unknown, 0,
                initial, (void**)&component) == 0 && component,
                "audit_create_for_each_held_interface")) continue;
        check(component->pVTbl->QueryInterface(component, &IID_IEcoUnknown,
            (void**)&before) == 0 && before, "audit_original_identity");
        release_interface(before);
        check(component->pVTbl->QueryInterface(component, ids[i],
            (void**)&held) == 0 && held, "audit_acquire_held_interface");
        release_interface(component);
        if (held) {
            check(mock.live > 0, "audit_each_interface_keeps_object_alive");
            check(held->pVTbl->QueryInterface(held, &IID_IEcoUnknown,
                (void**)&after) == 0 && after == before,
                "audit_identity_survives_initial_release");
            release_interface(after);
            matrix(held, lab2);
            audit_operational(held, lab2);
            release_interface(held);
        }
        check_balanced();
    }
}

static void audit_aggregate_hold_each(void) {
    const UGUID* ids[4];
    TestOuter outer;
    IEcoLab65* primary = 0;
    IEcoUnknown *held = 0, *canonical = 0;
    unsigned int i;
    ids[0] = &IID_IEcoUnknown;
    ids[1] = &IID_IEcoLab65;
    ids[2] = &IID_IEcoCalculatorX;
    ids[3] = &IID_IEcoCalculatorY;
    for (i = 0; i < 4; i++) {
        reset_mock();
        memset(&outer, 0, sizeof(outer));
        outer.unknown.pVTbl = &outer_vtable;
        outer.refs = 1;
        held = 0;
        primary = 0;
        if (!check(lab1_factory->pVTbl->Alloc(lab1_factory, &mock.unknown,
                &outer.unknown, &IID_IEcoUnknown, (void**)&outer.inner) == 0
                && outer.inner, "audit_external_aggregate_create")) continue;
        check(outer.refs == 1 && outer.adds == 0,
            "audit_no_borrowed_controller_cycle");
        outer.inner->pVTbl->QueryInterface(outer.inner, &IID_IEcoLab65,
            (void**)&primary);
        if (!check(primary != 0, "audit_external_primary")) {
            outer_release(&outer.unknown);
            continue;
        }
        check(primary->pVTbl->QueryInterface(primary, ids[i],
            (void**)&held) == 0 && held, "audit_external_held_interface");
        primary->pVTbl->Release(primary);
        outer_release(&outer.unknown);
        if (held) {
            check(outer.refs == 1 && mock.live > 0,
                "audit_external_each_interface_keeps_controller_alive");
            check(held->pVTbl->QueryInterface(held, &IID_IEcoUnknown,
                (void**)&canonical) == 0 && canonical == &outer.unknown,
                "audit_external_canonical_identity");
            release_interface(canonical);
            matrix(held, 0);
            audit_operational(held, 0);
            release_interface(held);
        }
        check(outer.refs == 0 && outer.inner == 0,
            "audit_external_final_release_destroys_aggregate");
        check_balanced();
    }
}

static void audit_optional_extension_and_isolation(int lab2) {
    IEcoComponentFactory* factory = lab2 ? lab2_factory : lab1_factory;
    const UGUID* initial = lab2 ? &IID_IEcoLab2 : &IID_IEcoLab65;
    IEcoUnknown *one = 0, *two = 0, *first_id = 0, *second_id = 0;
    reset_mock();
    mock.no_extension = 1;
    if (check(factory->pVTbl->Alloc(factory, &mock.unknown, 0, initial,
            (void**)&one) == 0 && one, "audit_optional_memext_absent")) {
        audit_operational(one, lab2);
        release_interface(one);
    }
    check_balanced();
    reset_mock();
    one = 0;
    two = 0;
    check(factory->pVTbl->Alloc(factory, &mock.unknown, 0, initial,
        (void**)&one) == 0 && one, "audit_first_independent_instance");
    check(factory->pVTbl->Alloc(factory, &mock.unknown, 0, initial,
        (void**)&two) == 0 && two, "audit_second_independent_instance");
    if (one && two) {
        one->pVTbl->QueryInterface(one, &IID_IEcoUnknown, (void**)&first_id);
        two->pVTbl->QueryInterface(two, &IID_IEcoUnknown, (void**)&second_id);
        check(first_id && second_id && first_id != second_id,
            "audit_distinct_instances_have_distinct_identity");
        release_interface(first_id);
        release_interface(second_id);
    }
    release_interface(one);
    if (two) audit_operational(two, lab2);
    release_interface(two);
    check_balanced();
}

int16_t EcoMain(IEcoUnknown* system) {
    int16_t status;
    unsigned int baseline;
    status = base_EcoMain(system);
    if (status != 0) return status;
    baseline = cases;
    audit_hold_each_public(0);
    audit_hold_each_public(1);
    audit_aggregate_hold_each();
    audit_optional_extension_and_isolation(0);
    audit_optional_extension_and_isolation(1);
    printf("EXTENDED_ACOM cases=%u extra=%u failures=%u mock_live=%u "
        "environment_refs=%u memory_refs=%u\n", cases, cases - baseline,
        errors, mock.live, mock.refs, mock.memory_refs);
    return errors ? 1 : 0;
}
