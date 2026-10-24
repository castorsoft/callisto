-- FUNCTION: public.fn_ncms_bu0()

-- DROP FUNCTION IF EXISTS public.fn_ncms_bu0();

CREATE OR REPLACE FUNCTION public.fn_ncms_bu0()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
	BEGIN
		NEW.atualizado_em := CURRENT_TIMESTAMP;
		RETURN NEW;
	END;
$BODY$;

-- Trigger: ncms_bu0

-- DROP TRIGGER IF EXISTS ncms_bu0 ON public.ncms;

CREATE OR REPLACE TRIGGER ncms_bu0
    BEFORE UPDATE
    ON public.ncms
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_ncms_bu0();